#include <commonLib/helpers.h>
#include <commonLib/client.h>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

UnixSockClient::UnixSockClient(std::string_view sockPath){
    m_remote.sun_family = AF_UNIX;
    std::strncpy(m_remote.sun_path, sockPath.data(), sizeof(m_remote.sun_path)-1);
    m_len = std::strlen(m_remote.sun_path) + sizeof(m_remote.sun_family);

}

UnixSockClient::~UnixSockClient(){
    ::close(m_sock);
    m_sock = -1;
}

void UnixSockClient::close(){
    if(m_sock > 0)
    {
        ::close(m_sock);
        m_sock = -1;
    }
}

bool UnixSockClient::init(){
    m_sock = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if(m_sock == -1)
    {
        helpers::dumpError("failed to open socket: ", errno);
        return false;
    }
    if(::connect(m_sock, (struct sockaddr *)&m_remote, static_cast<socklen_t>(m_len)) == -1) return false;

    return true;
}

bool UnixSockClient::send(const std::string && msg){

    std::string data = std::move(msg);
    data+="\n";
    if(m_sock > 0 && (::send(m_sock, data.data(), data.size(), MSG_NOSIGNAL) != -1))
    {
        return true;
    }
    close();
    init();

    return false;
}


