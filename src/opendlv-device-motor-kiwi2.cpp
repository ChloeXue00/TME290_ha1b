/*
 * Copyright (C) 2024 OpenDLV
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cmath>
#include <cstdint>
#include <iostream>

#include <pigpiod_if2.h>

#include "cluon-complete.hpp"
#include "opendlv-message-standard.hpp"

int32_t main(int32_t argc, char **argv)
{
  auto cmd = cluon::getCommandlineArguments(argc, argv);
  if (!cmd.contains("cid")) {
    std::cout << argv[0] << " is an OpenDLV microservice to run a Kiwi 2 motor"
      << std::endl;
    std::cout << "Usage: " << argv[0] << " "
      << "--cid=<conference id; e.g. 111> "
      << "[--verbose] "
      << std::endl;
    return 0;
  }

  uint16_t const cid = std::stoi(cmd.at("cid"));
  bool const verbose = (cmd.count("verbose") != 0);

  int32_t const pwmPin = 12;
  int32_t const dirPin = 16;
  int32_t const sleepPin = 7;

  uint32_t const minSpeed = 300;
  uint32_t const maxSpeed = 1600;

  int32_t const timeoutUs = 2e6;

  int32_t pi = pigpio_start(nullptr, nullptr);
  if (pi < 0) {
    std::cerr << "Could not start GPIO." << std::endl;
    return -1;
  }

  {
    int32_t r = set_mode(pi, pwmPin, PI_OUTPUT);
    if (r != 0) {
      std::cerr << "Could not assign pin " << pwmPin << "." << std::endl;
      pigpio_stop(pi);
      return -1;
    }
  }
  {
    int32_t r = set_mode(pi, dirPin, PI_OUTPUT);
    if (r != 0) {
      std::cerr << "Could not assign pin " << dirPin << "." << std::endl;
      pigpio_stop(pi);
      return -1;
    }
  }
  {
    int32_t r = set_mode(pi, sleepPin, PI_OUTPUT);
    if (r != 0) {
      std::cerr << "Could not assign pin " << sleepPin << "." << std::endl;
      pigpio_stop(pi);
      return -1;
    }
  }

  if (verbose) {
    std::cout << "Initializing motor." << std::endl;
  }
  set_PWM_range(pi, pwmPin, 100);
  
  cluon::OD4Session od4(cid);
  cluon::data::TimeStamp latestSpeed = cluon::time::now();
  bool gotFirst = false;

  auto onPedalPositionRequest{[&pi, &sleepPin, &dirPin, &pwmPin, &minSpeed,
    &maxSpeed, &latestSpeed, &gotFirst, &verbose](
        cluon::data::Envelope &&envelope) {

      latestSpeed = cluon::time::now();

      if (!gotFirst) {
        gotFirst = true;
        if (verbose) {
          std::cout << "Motor starting." << std::endl;
        }
      }

      gpio_write(pi, sleepPin, 0);

      auto const ppr =
        cluon::extractMessage<opendlv::proxy::PedalPositionRequest>(
            std::move(envelope));

      float pos = ppr.position();
      pos = std::min(pos, 1.0f);
      pos = std::max(pos, -1.0f);

      if (pos < 0.0f) {
        gpio_write(pi, dirPin, 0);
      } else {
        gpio_write(pi, dirPin, 1);
      }

      if (std::abs(pos) < 0.001f) {
        set_PWM_dutycycle(pi, pwmPin, 0);
        set_PWM_frequency(pi, pwmPin, 0);
      } else {
        set_PWM_dutycycle(pi, pwmPin, 95);
        uint32_t speed = minSpeed + 
          static_cast<uint32_t>(std::abs(pos) * (maxSpeed - minSpeed));
        set_PWM_frequency(pi, pwmPin, speed);
      }
  }};

  od4.dataTrigger(opendlv::proxy::PedalPositionRequest::ID(),
      onPedalPositionRequest);
  
  while (od4.isRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    uint32_t delayUs = cluon::time::toMicroseconds(cluon::time::now()) -
      cluon::time::toMicroseconds(latestSpeed);
    if (delayUs > timeoutUs) {
      gpio_write(pi, sleepPin, 1);
      set_PWM_dutycycle(pi, pwmPin, 0);
      set_PWM_frequency(pi, pwmPin, 0);
      if (verbose) {
        std::cout << "Motor timeout." << std::endl;
      }
      gotFirst = false;
    }
  }
  
  if (verbose) {
    std::cout << "Releasing motor." << std::endl;
  }

  gpio_write(pi, sleepPin, 1);
  set_PWM_dutycycle(pi, pwmPin, 0);
  set_PWM_frequency(pi, pwmPin, 0);
  
  pigpio_stop(pi);

  return 0;
}
