#include "net/Socket.h"

#include <utility>  // std::exchange

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

Socket::Socket(Socket&& other) noexcept
    // Move constructor: this object is being created to replace `other`.
    // Just steal the socket from `other`.
    : handle(std::exchange(other.handle, INVALID_SOCKET))
    , isShutDown(other.isShutDown.load())
{
}

Socket& Socket::operator=(Socket&& other) noexcept
{
    if (this != &other)
    {
        // Move assignment: this object is being replaced by `other`.
        // First close this socket, then steal the socket from `other`.
        close();
        handle = std::exchange(other.handle, INVALID_SOCKET);
        isShutDown = other.isShutDown.load();
    }
    return *this;
}

bool Socket::operator==(const Socket& other) const
{
    return handle == other.handle;
}

bool Socket::operator!=(const Socket& other) const
{
    return !(*this == other);
}

bool Socket::isOpen() const
{
    return handle != INVALID_SOCKET && !isShutDown;
}

}  // namespace Rival
