#pragma once
#include <string>
#include <functional>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class UnixSockServer final{
public:
    UnixSockServer() = default;
    ~UnixSockServer();

    UnixSockServer(UnixSockServer &) = delete;
    UnixSockServer(UnixSockServer &&) = delete;
    UnixSockServer& operator=(UnixSockServer&) = delete;
    UnixSockServer& operator=(UnixSockServer&&) = delete;


    bool init(const std::string & sockPath);
    void step(auto func){
            char str[100];
            int recvCount = ::recv(m_socketConection, str, 100, 0);
            if(recvCount == 0){
                ::close(m_socketConection);
                m_socketConection = -1;
                return;
            }
            str[recvCount] = 0;
            std::string message(str);
            func(message);
    }

    bool accepted();

private:
    std::string m_socketPath;
    int m_socketServer = -1;
    int m_socketConection = -1;
    sockaddr_un m_local;
    sockaddr_un m_remote;
};
