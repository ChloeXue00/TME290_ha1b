/*
 * Copyright (C) 2024 OpenDLV
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
    std::cout << argv[0] << " is an OpenDLV microservice." << std::endl;
    std::cout << "Usage: " << argv[0] << " "
              << "--cid=<conference id; e.g. 111> "
              << "[--verbose] " << std::endl;
    return 0;
  }

  uint16_t const cid = std::stoi(cmd.at("cid"));
  bool const verbose = (cmd.count("verbose") != 0);

  if (verbose) {
    std::cout << "Starting microservice." << std::endl;
  }

  cluon::OD4Session od4(cid);

  auto onGroundSpeedRequest{[&od4, &verbose](cluon::data::Envelope &&envelope) {
    auto const gsr =
        cluon::extractMessage<opendlv::proxy::GroundSpeedRequest>(
            std::move(envelope));

    float gs = gsr.groundSpeed();
    float multiplier = 2.5f;
    if (verbose) {
      std::cout << "Got ground Speed " << gs << std::endl;
    }

    opendlv::proxy::GroundSpeedReading reading;
    reading.groundSpeed(gs * multiplier);

    od4.send(reading);

    if (verbose){
      std::cout << "sent GroundSpeedReading: " << gs * multiplier << std::endl;
    }
  }};

  od4.dataTrigger(
      opendlv::proxy::GroundSpeedRequest::ID(), onGroundSpeedRequest);

  while (od4.isRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  if (verbose) {
    std::cout << "Closing microservice." << std::endl;
  }

  return 0;
}
