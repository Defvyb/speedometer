#include "commonLib/helpers.h"
#include <iostream>
#include <cstring>
#include <sys/un.h>

void helpers::log_warn(std::string_view message){
    std::cout << "Warning!: " << message << std::endl;
}

void helpers::log_error(std::string_view message){
    std::cout << "Error!: " << message << std::endl;
}

bool helpers::isCorrectSocketPath(std::string_view sockPath)
{
    if(sockPath.empty()){
        helpers::log_error("sock path is empty");
        return false;
    }

    if(std::strlen(sockPath.data()) > (sizeof(sockaddr_un::sun_path)-1)){
        helpers::log_error("path to socket arg is too big, consider path size < 107" + std::to_string((sizeof(sockaddr_un::sun_path)-1)));
        return false;
    }

    return true;
}
