#include <thread>
#include <iostream>
#include <signal.h>
#include <atomic>
#include <functional>

#include "options.hpp"
#include "utils.hpp"

#include "app/app.h"
#include "quantities/quantities_processor.h"
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
    server.init(options.socketPath);

    QuantitiesProcessor qProc;

    std::thread serverThread(
                [&quit, &server, &qProc](){
        while(!quit){
            if(server.accepted()){
                server.step([&qProc](const std::string & str){
                    try{
                        qProc.setExternalSpeed(std::stod(str));
                    }
                    catch(const std::exception & e){
                        helpers::log_error(e.what());
                    }
                });
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });

    std::thread processorThread(
                [&app, &quit, &qProc](){
                while(!quit){
                    qProc.processStep();
                    app.setSpeed(qProc.getCurrentSpeed());
                    app.setOdoKm(qProc.getOdoKm());
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
    }
    );

    processorThread.detach();
    serverThread.detach();
    appThread.join();
    if(processorThread.joinable()) processorThread.join();
    if(serverThread.joinable()) serverThread.join();


    return 0;
}
