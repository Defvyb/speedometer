#pragma once
#include <string>
#include <functional>
#include <sys/un.h>

class UnixSockServer final
{
public:
    UnixSockServer();
    ~UnixSockServer();

    bool init(const std::string & sockPath);
    void step(std::function<void(std::string & message)> func);
    bool accepted();

private:
    std::string m_socketPath;
    int m_socketServer;
    int m_socketConection;
    sockaddr_un m_local;
    sockaddr_un m_remote;

};
