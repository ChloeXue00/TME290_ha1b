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

#include "cluon-complete.hpp"
#include "opendlv-message-standard.hpp"

int32_t main(int32_t argc, char **argv)
{
  auto cmd = cluon::getCommandlineArguments(argc, argv);
  if (!cmd.contains("cid")) {
    std::cout << argv[0] << " is an OpenDLV microservice."
      << std::endl;
    std::cout << "Usage: " << argv[0] << " "
      << "--cid=<conference id; e.g. 111> "
      << "[--verbose] "
      << std::endl;
    return 0;
  }

  uint16_t const cid = std::stoi(cmd.at("cid"));
  bool const verbose = (cmd.count("verbose") != 0);

  if (verbose) {
    std::cout << "Starting microservice." << std::endl;
  }
  
  cluon::OD4Session od4(cid);

  auto onPedalPositionRequest{[&verbose](cluon::data::Envelope &&envelope) {
      auto const ppr =
        cluon::extractMessage<opendlv::proxy::PedalPositionRequest>(
            std::move(envelope));

      float pos = ppr.position();
      if (verbose) {
        std::cout << "Got pedal position " << pos << std::endl;
      }
  }};

  od4.dataTrigger(opendlv::proxy::PedalPositionRequest::ID(),
      onPedalPositionRequest);
  
  while (od4.isRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  
  if (verbose) {
    std::cout << "Closing microservice." << std::endl;
  }

  return 0;
}
