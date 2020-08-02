#pragma once

#include <string>
#include <sys/un.h>

class UnixSockClient final{
public:
    explicit UnixSockClient(std::string_view sockPath);
    ~UnixSockClient();

    UnixSockClient(UnixSockClient &) = delete;
    UnixSockClient(UnixSockClient &&) = delete;
    UnixSockClient& operator=(UnixSockClient&) = delete;
    UnixSockClient& operator=(UnixSockClient&&) = delete;

    bool init();
    bool send(const std::string && msg);
private:
    void close();

    int m_sock = -1;
    sockaddr_un m_remote;
    size_t m_len;
};




