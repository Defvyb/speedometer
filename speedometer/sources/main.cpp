#include <thread>
#include <iostream>

#include "options.hpp"
#include "utils.hpp"

#include "app/app.h"
#include <commonLib/server.h>
#include <commonLib/helpers.h>

int main( int argc, char* argv[] )
{
    std::set_terminate([](){ std::cout << "Unhandled exception\n"; std::abort();});

    ProgramOptions options;
    if(!getProgramOptions(argc, argv, options)) return -1;

    SDLAppSpeedometer app;
    app.init("speedometer");
    app.setSpeed(0);

    std::thread appThread(&SDLAppSpeedometer::run, &app, options.showFps);

    UnixSockServer server;
    server.init(options.socketAddr);

    std::atomic<bool> stopThread = false;
    std::thread speedThread(
                [&app, &stopThread, &server](){
        double i=0;
        bool front = true;
        while(!stopThread)
        {
            if(server.accepted())
            {
                server.step([&app](std::string & str)
                {
                    try
                    {
                        app.setSpeed(std::stod(str));
                    }
                    catch(const std::exception & e)
                    {
                        log_error(e.what());
                    }
                });
            }

        }


    });

    speedThread.detach();
    appThread.join();

    stopThread = true;
    if(speedThread.joinable()) speedThread.join();

    return 0;
}
