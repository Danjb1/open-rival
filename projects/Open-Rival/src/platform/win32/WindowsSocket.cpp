#ifdef _WIN32

// These comments...
#include "net/Socket.h"
// ... prevent the auto-formatter from moving the include

#include <winsock2.h>

#include <ws2tcpip.h>

#include <cassert>  // assert macro
#include <cstring>  // std::memset
#include <stdexcept>
#include <system_error>

#include "utils/LogUtils.h"

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
    const auto result = ::getaddrinfo(node, service, &hints, &out);

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
    const auto port_str = std::to_string(port);
    int flags = AI_PASSIVE | AI_NUMERICSERV;
    return lookupAddress(nullptr, port_str.data(), domain, type, protocol, flags);
}

/** Gets the addrInfo for connecting to a server. */
addrInfoPtr getAddressInfo(int domain, int type, int protocol, std::string const& node, std::uint16_t port)
{
    const auto port_str = std::to_string(port);
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
    const auto addrInfoPtr = getLocalAddressInfo(domain, type, protocol, port);

    // Create
    SOCKET handle = ::socket(domain, type, protocol);
    if (handle == INVALID_SOCKET)
    {
        const auto err = ::WSAGetLastError();
        throw std::runtime_error("Failed to create socket: " + std::to_string(err));
    }

    // Bind
    const auto bindResult = ::bind(handle, addrInfoPtr->ai_addr, static_cast<int>(addrInfoPtr->ai_addrlen));
    if (bindResult == SOCKET_ERROR)
    {
        const auto err = ::WSAGetLastError();
        ::closesocket(handle);
        throw std::runtime_error("Failed to bind socket: " + std::to_string(err));
    }

    // Listen
    if (::listen(handle, SOMAXCONN) == SOCKET_ERROR)
    {
        const auto err = ::WSAGetLastError();
        ::closesocket(handle);
        throw std::runtime_error("Failed to listen on socket: " + std::to_string(err));
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
    const auto connectResult = ::connect(handle, addrInfoPtr->ai_addr, static_cast<int>(addrInfoPtr->ai_addrlen));
    if (connectResult == SOCKET_ERROR)
    {
        const auto err = ::WSAGetLastError();
        ::closesocket(handle);
        throw std::runtime_error("Failed to connect to server: " + std::to_string(err));
    }

    return { handle };
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Socket Implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** How often blocking calls check whether the socket has been shut down. */
static constexpr long pollIntervalMicros = 100'000;  // 100ms

void Socket::init()
{
    // Disable Nagle algorithm to ensure packets are not held up
    BOOL socketOptionValue = TRUE;
    ::setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (char*) (&socketOptionValue), sizeof(BOOL));
}

void Socket::shutdown() noexcept
{
    if (isShutDown.exchange(true) || handle == INVALID_SOCKET)
    {
        // Already shut down, or there is nothing to shut down
        return;
    }

    // Let the other side know that the connection is ending.
    // This fails for listening sockets, since they are not connected, but this is harmless.
    // Note that, on Windows, this does NOT wake up blocking calls on other threads; instead, those calls periodically
    // check whether the socket has been shut down (see `waitUntilReady`).
    ::shutdown(handle, SD_BOTH);
}

/**
 * Blocking call that waits until the socket is ready (for either reading or writing).
 * This is used as a safety mechanism before calling various built-in blocking methods (e.g. `accept`, `send`, `recv`),
 * because it reliably detects whether the socket has been closed and returns, instead of blocking indefinitely.
 */
bool Socket::waitUntilReady(bool forWriting) const
{
    // Keep polling the socket as long as it is open
    while (isOpen())
    {
        // Create an `fd_set` and populate it with our socket handle
        ::fd_set socketSet;
        FD_ZERO(&socketSet);
        FD_SET(handle, &socketSet);

        // Call `select` to check the socket status, with a timeout to ensure we don't block forever
        ::fd_set* readSet = forWriting ? nullptr : &socketSet;
        ::fd_set* writeSet = forWriting ? &socketSet : nullptr;
        ::timeval timeout = { 0, pollIntervalMicros };
        const int result = ::select(0, readSet, writeSet, nullptr, &timeout);

        if (result > 0)
        {
            // Socket is ready!
            return true;
        }

        if (result == SOCKET_ERROR)
        {
            LOG_WARN("Failed to wait for socket: {}", ::WSAGetLastError());
            return false;
        }

        // Timed out; now is our chance to return if the socket was closed
    }

    return false;
}

void Socket::close() noexcept
{
    if (handle == INVALID_SOCKET)
    {
        // Already closed
        return;
    }

    const auto result = ::closesocket(handle);
    if (result != 0)
    {
        LOG_WARN("Failed to close socket: {}", ::WSAGetLastError());
    }

    handle = INVALID_SOCKET;
}

std::uint16_t Socket::getLocalPort() const
{
    ::sockaddr_in address = {};
    int addressSize = sizeof(address);
    if (::getsockname(handle, reinterpret_cast<::sockaddr*>(&address), &addressSize) != 0)
    {
        return 0;
    }
    return ::ntohs(address.sin_port);
}

Socket Socket::accept()
{
    // A listening socket is "ready to read" when there is a connection waiting
    if (!waitUntilReady(false))
    {
        return {};
    }

    const SOCKET clientSocket = ::accept(handle, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET)
    {
        LOG_WARN("Failed to accept client: {}", ::WSAGetLastError());
        return {};
    }

    return { clientSocket };
}

void Socket::send(const std::vector<char>& buffer)
{
    std::size_t bytesSent = 0;

    while (bytesSent < buffer.size())
    {
        if (!waitUntilReady(true))
        {
            // Shut down while waiting, or an error occurred
            shutdown();
            break;
        }

        std::size_t bytesRemaining = buffer.size() - bytesSent;
        int result = ::send(handle, buffer.data() + bytesSent, static_cast<int>(bytesRemaining), 0);

        if (result == SOCKET_ERROR)
        {
            if (isOpen())
            {
                // Socket is still open on our side but may have been closed by the other side
                LOG_WARN("Failed to send on socket: {}", ::WSAGetLastError());
                shutdown();
            }

            // Otherwise, the socket was shut down on our side (e.g. by another thread); either way, we can't send
            break;
        }

        bytesSent += result;
    }
}

static std::string getWinSockErrorString(int error)
{
    char* buffer = nullptr;

    const DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    const DWORD length = FormatMessageA(flags, nullptr, error, 0, reinterpret_cast<LPSTR>(&buffer), 0, nullptr);
    if (length == 0 || buffer == nullptr)
    {
        return "WSA error " + std::to_string(error);
    }

    std::string message(buffer, length);
    LocalFree(buffer);

    return message;
}

static void handleSocketError()
{
    switch (const int error = ::WSAGetLastError())
    {
    case WSAECONNABORTED:
        LOG_DEBUG("Socket closed locally (connection aborted)");
        break;

    case WSAESHUTDOWN:
        LOG_DEBUG("Socket shut down locally");
        break;

    case WSAECONNRESET:
        LOG_INFO("Socket closed by peer (connection reset)");
        break;

    default:
        LOG_WARN("Unexpected error reading from socket: {}", getWinSockErrorString(error));
        break;
    }
}

void Socket::receive(std::vector<char>& buffer)
{
    std::size_t bytesReceived = 0;

    while (bytesReceived < buffer.size())
    {
        if (!waitUntilReady(false))
        {
            // Shut down while waiting, or an error occurred
            shutdown();
            break;
        }

        std::size_t bytesExpected = buffer.size() - bytesReceived;
        int result = ::recv(handle, buffer.data() + bytesReceived, static_cast<int>(bytesExpected), 0);

        if (result > 0)
        {
            bytesReceived += result;
            continue;
        }

        if (result == SOCKET_ERROR && isOpen())
        {
            handleSocketError();
        }

        // The connection has been closed by the other side, or shut down on our side (e.g. by another thread)
        shutdown();
        break;
    }
}

}  // namespace Rival

#endif
