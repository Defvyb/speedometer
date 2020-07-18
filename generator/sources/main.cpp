#include <commonLib/client.h>
#include <cmath>
#include <thread>
int main( int argc, char* argv[] )
{
    if(argc != 2)
    {
        printf("usage: use second arg - path to socket: generator /tmp/socktmp.sock");
        return -1;
    }

    UnixSockClient client(argv[1]);
    client.init();

    double i=0.0;
    while(true)
    {
        i++;
        if(!client.send(std::to_string(fabs(sin(i/600.0)+sin((i+20.0)/134.0))*150.0)))
        {
            client.close();
            client.init();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
