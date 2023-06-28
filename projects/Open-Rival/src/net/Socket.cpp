#include "net/Socket.h"

namespace Rival {

Socket::Socket()
    : handle(INVALID_SOCKET)
{
}

Socket::Socket(SOCKET handle)
    : handle(handle)
{
    init();
}

Socket::~Socket()
{
    close();
}

bool Socket::operator==(const Socket& other) const
{
    return handle == other.handle;
}

bool Socket::operator!=(const Socket& other) const
{
    return !(*this == other);
}

}  // namespace Rival
