#include <commonLib/client.h>
#include <commonLib/helpers.h>
#include <cmath>
#include <thread>
#include <functional>
#include <signal.h>

std::function<void(int)>  shutdownHandler;

void signalHandler(int signal){
    shutdownHandler(signal);
}

int main(int argc, char* argv[]){
    bool quit = false;
    shutdownHandler = [&quit](int){quit = true;};

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    if(argc != 2){
        helpers::log_warn("usage: use second arg - path to socket: generator /tmp/socktmp.sock");
        return -1;
    }

    if(!helpers::isCorrectSocketPath(argv[1])) return -1;

    UnixSockClient client(argv[1]);
    client.init();

    double i=0.0;
    while(!quit){
        i++;
        client.send(std::to_string( (fabs(sin(i/600.0)+sin((i+20.0)/134.0))*150.0) + (std::fmod(i,2.0) == 0.0 ? 5.0: -5.0)) );
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
