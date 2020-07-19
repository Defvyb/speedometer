#include <thread>
#include <iostream>
#include <signal.h>
#include <atomic>
#include <functional>

#include "options.hpp"
#include "utils.hpp"

#include "app/app.h"
#include <commonLib/server.h>
#include <commonLib/helpers.h>

std::function<void(int)> shutdownHandler;
void signalHandler(int signal){
    shutdownHandler(signal);
}

int main(int argc, char* argv[]){
    std::atomic<bool> quit = false;
    shutdownHandler = [&quit](int){quit = true;};
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    std::set_terminate([](){ std::cout << "Unhandled exception\n"; std::abort();});

    ProgramOptions options;
    if(!getProgramOptions(argc, argv, options)) return -1;

    SDLAppSpeedometer app;
    app.init("speedometer");
    app.setSpeed(0);

    std::thread appThread(&SDLAppSpeedometer::run, &app, options.showFps, std::ref(quit));

    UnixSockServer server;
    server.init(options.socketAddr);

    std::thread speedThread(
                [&app, &quit, &server](){
        while(!quit){
            if(server.accepted()){
                server.step([&app](const std::string & str){
                    try{
                        app.setSpeed(std::stod(str));
                    }
                    catch(const std::exception & e){
                        log_error(e.what());
                    }
                });
            }
        }
    });

    speedThread.detach();
    appThread.join();
    if(speedThread.joinable()) speedThread.join();

    return 0;
}
