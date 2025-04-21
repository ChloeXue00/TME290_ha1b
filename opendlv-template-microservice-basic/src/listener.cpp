#include "cluon-complete.hpp"
#include "opendlv-message-standard.hpp"

#include <iostream>
#include <chrono>
#include <iomanip>  // for std::put_time

int32_t main(int32_t argc, char **argv)
{
  auto cmd = cluon::getCommandlineArguments(argc, argv);
  if (!cmd.contains("cid")) {
    std::cerr << "Usage: " << argv[0] << " --cid=<conference id>" << std::endl;
    return 1;
  }

  uint16_t const cid = std::stoi(cmd.at("cid"));
  float const multiplier = 2.5f;  // the same as microservice 

  cluon::OD4Session od4(cid, [multiplier](cluon::data::Envelope &&envelope) {
    if (envelope.dataType() == opendlv::proxy::GroundSpeedReading::ID()) {
      auto const msg = cluon::extractMessage<opendlv::proxy::GroundSpeedReading>(std::move(envelope));

      //current time stamp
      auto now = std::chrono::system_clock::now();
      auto t_c = std::chrono::system_clock::to_time_t(now);
      std::cout << "\n Message received at " << std::put_time(std::localtime(&t_c), "%F %T") << std::endl;

      float reading = msg.groundSpeed();
      float original = reading / multiplier;

      std::cout << "GroundSpeedReading.groundSpeed = " << reading << std::endl;
      std::cout << "Take GroundSpeedRequest.groundSpeed = " << original
                << " times*  " << multiplier << " to get the value" << std::endl;
    }
  });

  std::cout << "Listening for GroundSpeedReading on CID " << cid << "..." << std::endl;

  while (od4.isRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
