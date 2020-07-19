#include "commonLib/server.h"
#include <commonLib/helpers.h>
#include <cstring>

UnixSockServer::~UnixSockServer(){
    ::close(m_socketConection);
    ::close(m_socketServer);
    m_socketConection = -1;
    m_socketServer = -1;
    unlink(m_local.sun_path);
}

bool UnixSockServer::init(const std::string & sockPath){
    if ((m_socketServer = ::socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
        helpers::log_error("Failed to create server socket");
        return false;
    }

    m_local.sun_family = AF_UNIX;
    std::strncpy(m_local.sun_path, sockPath.c_str(), sizeof(m_local.sun_path)-1);

    unlink(m_local.sun_path);
    size_t length = std::strlen(m_local.sun_path) + sizeof(m_local.sun_family);

    if (::bind(m_socketServer, (struct sockaddr *)&m_local, static_cast<socklen_t>(length)) == -1){
        helpers::log_error("failed to bind server socket");
        return false;
    }

    if (::listen(m_socketServer, 1) == -1){
        helpers::log_error("failed to listen server socket");
        return false;
    }

    return true;
}

bool UnixSockServer::accepted(){
    unsigned int sizeRemote = sizeof(m_remote);
    if(m_socketConection > 0) return true;
    if ((m_socketConection = ::accept(m_socketServer, (struct sockaddr *)&m_remote, &sizeRemote)) == -1){
        ::close(m_socketConection);
        m_socketConection = -1;
        return false;
    }

    return true;
}

