#pragma once

#include <string>
#include <iostream>
#include <options.hpp>
#include <commonLib/helpers.h>

void printHelp(){
    std::cout << "Usage: -fps show fps \n";
    std::cout << "Usage: -sock_name=/tmp/socktmp.sock socket address to open \n";
    std::cout << "       -h this help  \n";
}

bool getProgramOptions(int argc, char *argv[],  ProgramOptions & options) noexcept{

    for(int i=1; i< argc; i++){
        std::string arg(argv[i]);
        auto findResult = arg.find("-h");
        if(findResult != std::string::npos){
           printHelp();
           return false;
        }

        findResult = arg.find("-fps");
        if(findResult != std::string::npos){
            options.showFps = true;
        }

        findResult = arg.find("-sock_name=");
        if(findResult != std::string::npos){
            std::string sockPath = arg.substr(arg.find_first_of("=")+1);
            if(!helpers::isCorrectSocketPath(sockPath)) return false;
            options.socketPath = sockPath;
        }
    }
    return true;
}
