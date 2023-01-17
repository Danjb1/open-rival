#include "pch.h"

#include "net/Socket.h"

namespace Rival {

Socket Socket::createServer(int port)
{
    return Socket("localhost", port, true);
}

Socket Socket::createClient(const std::string& address, int port)
{
    return Socket(address, port, false);
}

std::shared_ptr<Socket> Socket::wrap(SOCKET rawSocket)
{
    return std::make_shared<Socket>(rawSocket);
}

Socket::Socket(SOCKET rawSocket)
    : sock(rawSocket)
{
    init();
}

Socket::~Socket()
{
    close();
}

bool Socket::isClosed() const
{
    return state == SocketState::Closed;
}

}  // namespace Rival
