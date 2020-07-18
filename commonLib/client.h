#pragma once

#include <string>
#include <sys/un.h>

class UnixSockClient final
{
public:
    UnixSockClient(std::string_view sockPath);
    ~UnixSockClient();
    bool init();
    bool send(std::string_view msg);
    void close();
private:
    int m_sock;
    sockaddr_un m_remote;
    int m_len;
};




