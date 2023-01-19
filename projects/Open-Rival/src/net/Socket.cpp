#include "pch.h"

#include "net/Socket.h"

namespace Rival {

std::unique_ptr<Socket> Socket::createServer(int port)
{
    return std::make_unique<Socket>("localhost", port, true);
}

std::unique_ptr<Socket> Socket::createClient(const std::string& address, int port)
{
    return std::make_unique<Socket>(address, port, false);
}

std::unique_ptr<Socket> Socket::wrap(SOCKET rawSocket)
{
    return std::make_unique<Socket>(rawSocket);
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
