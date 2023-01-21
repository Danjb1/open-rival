#include "pch.h"

#include "net/Socket.h"

namespace Rival {

Socket Socket::createServer(int port)
{
    return { "localhost", port, true };
}

Socket Socket::createClient(const std::string& address, int port)
{
    return { address, port, false };
}

Socket Socket::wrap(SOCKET rawSocket)
{
    return { rawSocket };
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
