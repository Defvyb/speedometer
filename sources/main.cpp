#include "app/app.h"
#include <thread>

int main( int argc, char* args[] )
{
    SDLAppSpeedometer app;
    app.init("speedometer");
    app.setSpeed(0);

    std::thread appThread(&SDLAppSpeedometer::run, &app);

    std::atomic<bool> stopThread = false;
    std::thread speedThread(
                [&app, &stopThread](){
        double i=0;
        bool front = true;
        while(!stopThread)
        {
            if(front)
            {
               app.setSpeed(i+=0.1);
            }
            else
            {
                app.setSpeed(i-=0.1);
            }

            if(i>320)
            {
                front = false;
            }

            if(i<0)
            {
                front = true;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1));

        }


    });

    speedThread.detach();
    appThread.join();

    stopThread = true;
    if(speedThread.joinable()) speedThread.join();

    return 0;
}
