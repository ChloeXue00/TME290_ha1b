#include "cluon-complete.hpp"
#include "opendlv-message-standard.hpp"

#include <chrono>
#include <thread>
#include <iostream>

int32_t main(int32_t argc, char **argv)
{
    auto cmd  = cluon::getCommandlineArguments(argc,argv);
    if(!cmd.contains("cid")){
        std::cerr << "Usage: " << argv[0] << " --cid=<conference id>" << std::endl;
        return 1;
    }

    uint16_t const cid = std::stoi(cmd.at("cid"));
    cluon::OD4Session od4(cid);

    std::cout << "GroundSpeedRequest is sending, (cid= " << cid  << ")" << std::endl;

    while (od4.isRunning()){
        std::cout << "input a groundspeed : " ;
        float value;
        std::cin>>value;

        if (std::cin.fail()){
            std::cerr << "input failed, input a number" <<std::endl;
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            continue;
        }
        opendlv::proxy::GroundSpeedRequest gsr;
        gsr.groundSpeed(value);

        od4.send(gsr);
        std::cout << "sent GroundSpeedRequest with value: " << value << std::endl;
    }
    
    return 0;
}