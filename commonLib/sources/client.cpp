#include <commonLib/helpers.h>
#include <commonLib/client.h>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>


UnixSockClient::UnixSockClient(std::string_view sockPath):m_sock(-1)
{
    m_remote.sun_family = AF_UNIX;
    std::strcpy(m_remote.sun_path, sockPath.data());
    m_len = std::strlen(m_remote.sun_path) + sizeof(m_remote.sun_family);

}
UnixSockClient::~UnixSockClient()
{
    ::close(m_sock);
    m_sock = -1;
}
void UnixSockClient::close()
{
    if(m_sock > 0)
    {
        ::close(m_sock);
        m_sock = -1;
    }

}
bool UnixSockClient::init()
{
    m_sock = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if(m_sock == -1)
    {
        printf("failed to open socket, errno %d\n", errno);
        return false;
    }
    if(::connect(m_sock, (struct sockaddr *)&m_remote, m_len) == -1) return false;

    return true;
}
bool UnixSockClient::send(std::string_view msg)
{
   /* int error;
    uint32_t errorSize = sizeof(error);
    getsockopt(m_sock, SOL_SOCKET, SO_ERROR, &error, &errorSize );
    if(error)
    {
        return false;
    }*/
    if(m_sock > 0)
    {
        if(::send(m_sock, msg.data(), std::strlen(msg.data()), MSG_NOSIGNAL) != -1) return true;
    }

    return false;
}


