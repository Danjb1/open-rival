#ifdef _WIN32

// These comments...
#include "net/Socket.h"
// ... prevent the auto-formatter from moving the include

#include <winsock2.h>

#include <ws2tcpip.h>

#include <cassert>  // assert macro
#include <cstring>  // std::memset
#include <iostream>
#include <stdexcept>
#include <utility>  // std::exchange

namespace Rival {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Address Utiltiies
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This is a std::unique_ptr to a `::addrinfo` that accepts a custom deletor, with the signature of `::freeaddrinfo`.
// This allows us to create addrinfo instances that clean themselves up when they go out of scope.
using addrInfoPtr = std::unique_ptr<::addrinfo, decltype(&::freeaddrinfo)>;

addrInfoPtr lookupAddress(char const* node, char const* service, int domain, int type, int protocol, int flags)
{
    assert(node || service);

    auto hints = ::addrinfo();
    std::memset(&hints, 0, sizeof(::addrinfo));
    hints.ai_family = domain;
    hints.ai_socktype = type;
    hints.ai_protocol = protocol;
    hints.ai_flags = flags;

    auto out = (::addrinfo*) nullptr;
    auto const result = ::getaddrinfo(node, service, &hints, &out);

    if (result != 0)
    {
        throw std::system_error(std::error_code(result, std::system_category()));
    }

    assert(out);

    return addrInfoPtr(out, &::freeaddrinfo);
}

/** Gets the addrInfo for hosting a local server. */
addrInfoPtr getLocalAddressInfo(int domain, int type, int protocol, std::uint16_t port)
{
    auto const port_str = std::to_string(port);
    int flags = AI_PASSIVE | AI_NUMERICSERV;
    return lookupAddress(nullptr, port_str.data(), domain, type, protocol, flags);
}

/** Gets the addrInfo for connecting to a server. */
addrInfoPtr getAddressInfo(int domain, int type, int protocol, std::string const& node, std::uint16_t port)
{
    auto const port_str = std::to_string(port);
    int flags = AI_NUMERICSERV;
    return lookupAddress(node.data(), port_str.data(), domain, type, protocol, flags);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Factory Methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Socket Socket::createServer(std::uint16_t port)
{
    const auto domain = AF_INET;
    const auto type = SOCK_STREAM;
    const auto protocol = IPPROTO_TCP;
    auto const addrInfoPtr = getLocalAddressInfo(domain, type, protocol, port);

    // Create
    SOCKET handle = ::socket(domain, type, protocol);
    if (handle == INVALID_SOCKET)
    {
        const auto err = ::WSAGetLastError();
        throw std::runtime_error("Failed to create socket: " + std::to_string(err));
    }

    // Bind
    const auto bindResult = ::bind(handle, addrInfoPtr->ai_addr, addrInfoPtr->ai_addrlen);
    if (bindResult == SOCKET_ERROR)
    {
        const auto err = ::WSAGetLastError();
        throw std::runtime_error("Failed to bind socket: " + std::to_string(err));
    }

    // Listen
    if (::listen(handle, SOMAXCONN) == SOCKET_ERROR)
    {
        throw std::runtime_error("Failed to listen on socket: " + std::to_string(::WSAGetLastError()));
    }

    return { handle };
}

Socket Socket::createClient(const std::string& address, std::uint16_t port)
{
    const auto domain = AF_INET;
    const auto type = SOCK_STREAM;
    const auto protocol = IPPROTO_TCP;
    const auto addrInfoPtr = getAddressInfo(domain, type, protocol, address, port);

    // Create
    SOCKET handle = ::socket(domain, type, protocol);
    if (handle == INVALID_SOCKET)
    {
        int err = ::WSAGetLastError();
        throw std::runtime_error("Failed to create socket: " + std::to_string(err));
    }

    // Connect
    const auto connectResult = ::connect(handle, addrInfoPtr->ai_addr, addrInfoPtr->ai_addrlen);
    if (connectResult == SOCKET_ERROR)
    {
        const auto err = ::WSAGetLastError();
        throw std::runtime_error("Failed to connect to server: " + std::to_string(err));
    }

    return { handle };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Socket Implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Socket::Socket(Socket&& other) noexcept
    // Move constructor: this object is being created to replace `other`.
    // Just steal the socket from `other`.
    : handle(std::exchange(other.handle, INVALID_SOCKET))
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
    }
    return *this;
}

void Socket::init()
{
    // Disable Nagle algorithm to ensure packets are not held up
    BOOL socketOptionValue = TRUE;
    ::setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (char*) (&socketOptionValue), sizeof(BOOL));
}

void Socket::close() noexcept
{
    if (!isOpen())
    {
        return;
    }

    auto const result = ::closesocket(handle);
    if (result != 0)
    {
        std::cerr << "Failed to close socket: " + std::to_string(::WSAGetLastError()) << "\n";
    }

    handle = INVALID_SOCKET;
}

Socket Socket::accept()
{
    SOCKET clientSocket = INVALID_SOCKET;

    clientSocket = ::accept(handle, nullptr, nullptr);

    if (clientSocket == INVALID_SOCKET && isOpen())
    {
        std::cerr << "Failed to accept client: " + std::to_string(::WSAGetLastError()) << "\n";
    }

    return { clientSocket };
}

bool Socket::isOpen() const
{
    return handle != INVALID_SOCKET;
}

void Socket::send(const std::vector<char>& buffer)
{
    std::size_t bytesSent = 0;

    while (bytesSent < buffer.size())
    {
        std::size_t bytesRemaining = buffer.size() - bytesSent;
        int result = ::send(handle, buffer.data() + bytesSent, bytesRemaining, 0);

        if (result == SOCKET_ERROR && isOpen())
        {
            // Socket is still open on our side but may have been closed by the other side
            std::cerr << "Failed to send on socket: " + std::to_string(::WSAGetLastError()) << "\n";
            close();
            break;
        }

        bytesSent += result;
    }
}

void Socket::receive(std::vector<char>& buffer)
{
    std::size_t bytesReceived = 0;

    while (bytesReceived < buffer.size())
    {
        std::size_t bytesExpected = buffer.size() - bytesReceived;
        int result = ::recv(handle, buffer.data() + bytesReceived, bytesExpected, 0);

        if (result == SOCKET_ERROR && isOpen())
        {
            // Socket is still open on our side but may have been closed by the other side
            std::cerr << "Failed to read from socket: " + std::to_string(::WSAGetLastError()) << "\n";
            close();
            break;
        }

        if (result == 0)
        {
            // Connection has been gracefully closed
            close();
            break;
        }

        bytesReceived += result;
    }
}

}  // namespace Rival

#endif
