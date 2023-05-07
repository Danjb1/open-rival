#include "pch.h"

#ifdef _WIN32

// These comments...
#include "net/Socket.h"
// ... prevent the auto-formatter from moving the include

#include <winsock2.h>

#include <ws2tcpip.h>

#include <iostream>
#include <stdexcept>
#include <utility>  // std::exchange

namespace Rival {

Socket::Socket()
    : sock(INVALID_SOCKET)
{
}

Socket::Socket(const std::string& address, int port, bool server)
{
    // Specify socket properties
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (server)
    {
        hints.ai_flags = AI_PASSIVE;
    }

    // Resolve the local address and port to be used by the server
    addrinfo* addrInfo = nullptr;
    if (int err = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &addrInfo))
    {
        throw std::runtime_error("Failed to get net address: " + std::to_string(err));
    }

    // Create the socket
    sock = INVALID_SOCKET;
    sock = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);

    // Check for errors
    if (sock == INVALID_SOCKET)
    {
        int err = WSAGetLastError();
        freeaddrinfo(addrInfo);
        throw std::runtime_error("Failed to create socket: " + std::to_string(err));
    }

    // Common socket initialization
    init();

    if (server)
    {
        // Bind the socket
        int bindResult = bind(sock, addrInfo->ai_addr, static_cast<int>(addrInfo->ai_addrlen));
        if (bindResult == SOCKET_ERROR)
        {
            int err = WSAGetLastError();
            freeaddrinfo(addrInfo);
            throw std::runtime_error("Failed to bind socket: " + std::to_string(err));
        }

        // Address info is no longer needed
        freeaddrinfo(addrInfo);

        // Listen (server-only)
        if (listen(sock, SOMAXCONN) == SOCKET_ERROR)
        {
            throw std::runtime_error("Failed to listen on socket: " + std::to_string(WSAGetLastError()));
        }
    }
    else
    {
        // Connect to server
        int connectResult = connect(sock, addrInfo->ai_addr, static_cast<int>(addrInfo->ai_addrlen));
        int err = WSAGetLastError();
        if (connectResult == SOCKET_ERROR)
        {
            sock = INVALID_SOCKET;
        }

        // Address info is no longer needed
        freeaddrinfo(addrInfo);

        if (sock == INVALID_SOCKET)
        {
            throw std::runtime_error("Failed to connect to server: " + std::to_string(err));
        }
    }
}

Socket::Socket(Socket&& other) noexcept
    // Move constructor: this object is being created to replace `other`.
    // Just steal the socket from `other`.
    : sock(std::exchange(other.sock, INVALID_SOCKET))
{
}

Socket& Socket::operator=(Socket&& other)
{
    if (this != &other)
    {
        // Move assignment: this object is being replaced by `other`.
        // First close this socket, then steal the socket from `other`.
        close();
        sock = std::exchange(other.sock, INVALID_SOCKET);
    }
    return *this;
}

void Socket::init()
{
    // Disable Nagle algorithm to ensure packets are not held up
    BOOL socketOptionValue = TRUE;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*) (&socketOptionValue), sizeof(BOOL));
}

void Socket::close() noexcept
{
    if (!isOpen())
    {
        return;
    }

    closesocket(sock);
    sock = INVALID_SOCKET;
}

Socket Socket::accept()
{
    SOCKET clientSocket = INVALID_SOCKET;

    clientSocket = ::accept(sock, nullptr, nullptr);

    if (clientSocket == INVALID_SOCKET && isOpen())
    {
        std::cout << "Failed to accept client: " + std::to_string(WSAGetLastError()) << "\n";
    }

    return Socket::wrap(clientSocket);
}

bool Socket::isOpen() const
{
    return sock != INVALID_SOCKET;
}

void Socket::send(const std::vector<char>& buffer)
{
    std::size_t bytesSent = 0;

    while (bytesSent < buffer.size())
    {
        std::size_t bytesRemaining = buffer.size() - bytesSent;
        int result = ::send(sock, buffer.data() + bytesSent, bytesRemaining, 0);

        if (result == SOCKET_ERROR && isOpen())
        {
            // Socket is still open on our side but may have been closed by the other side
            std::cerr << "Failed to send on socket: " + std::to_string(WSAGetLastError()) << "\n";
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
        int result = ::recv(sock, buffer.data() + bytesReceived, bytesExpected, 0);

        if (result == SOCKET_ERROR && isOpen())
        {
            // Socket is still open on our side but may have been closed by the other side
            std::cerr << "Failed to read from socket: " + std::to_string(WSAGetLastError()) << "\n";
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
