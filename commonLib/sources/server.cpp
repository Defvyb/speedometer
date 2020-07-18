#include "commonLib/server.h"
#include <commonLib/helpers.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
UnixSockServer::UnixSockServer():m_socketServer(-1), m_socketConection(-1)
{
}
UnixSockServer::~UnixSockServer()
{
    ::close(m_socketConection);
    ::close(m_socketServer);
    m_socketConection = -1;
    m_socketServer = -1;
    unlink(m_local.sun_path);


}

bool UnixSockServer::init(const std::string & sockPath)
{
    if ((m_socketServer = ::socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        log_error("Failed to create server socket");
        return false;
    }
    m_local.sun_family = AF_UNIX;
    std::strcpy(m_local.sun_path, sockPath.c_str());

    unlink(m_local.sun_path);
    int length = std::strlen(m_local.sun_path) + sizeof(m_local.sun_family);

    if (::bind(m_socketServer, (struct sockaddr *)&m_local, length) == -1)
    {
        log_error("failed to bind server socket");
        return false;
    }

    if (::listen(m_socketServer, 1) == -1)
    {
        log_error("failed to listen server socket");
        return false;
    }
    return true;
}

bool UnixSockServer::accepted()
{
    unsigned int sizeRemote = sizeof(m_remote);
    if(m_socketConection > 0) return true;
    if (((m_socketConection = ::accept(m_socketServer, (struct sockaddr *)&m_remote, &sizeRemote)) == -1))
    {

        ::close(m_socketConection);
        m_socketConection = -1;

        return false;
    }

    return true;

}
void UnixSockServer::step(std::function<void(std::string & message)> func)
{
        char str[100];
        int recvCount = ::recv(m_socketConection, str, 100, 0);
        if(recvCount == 0)
        {
            ::close(m_socketConection);
            m_socketConection = -1;
            return;
        }
        str[recvCount] = 0;
        std::string message(str);
        func(message);
}

