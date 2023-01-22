#include "pch.h"

#ifdef _WIN32

// These comments...
#include "net/Socket.h"
// ... prevent the auto-formatter from moving the include

#include <winsock2.h>

#include <ws2tcpip.h>

#include <iostream>
#include <stdexcept>

namespace Rival {

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

    if (isValid())
    {
        state = SocketState::Open;
    }
}

Socket::Socket(Socket&& other) noexcept
    : sock(other.sock)
    , state(other.state)
{
    // Reset the source object so its destructor is harmless
    other.sock = INVALID_SOCKET;
    other.state = SocketState::Closed;
}

void Socket::init()
{
    // Disable Nagle algorithm to ensure packets are not held up
    BOOL socketOptionValue = TRUE;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*) (&socketOptionValue), sizeof(BOOL));
}

void Socket::close() noexcept
{
    if (state == SocketState::Closed)
    {
        return;
    }

    state = SocketState::Closed;
    closesocket(sock);
}

Socket Socket::accept()
{
    SOCKET clientSocket = INVALID_SOCKET;

    clientSocket = ::accept(sock, nullptr, nullptr);

    if (clientSocket == INVALID_SOCKET && !isClosed())
    {
        std::cout << "Failed to accept client: " + std::to_string(WSAGetLastError()) << "\n";
    }

    return Socket::wrap(clientSocket);
}

bool Socket::isValid() const
{
    return sock != INVALID_SOCKET;
}

void Socket::send(std::vector<char>& buffer)
{
    int result = ::send(sock, buffer.data(), buffer.size(), 0);
    if (result == SOCKET_ERROR)
    {
        throw std::runtime_error("Failed to send on socket: " + std::to_string(WSAGetLastError()));
    }
}

void Socket::receive(std::vector<char>& buffer)
{
    // Receive until the peer shuts down the connection
    int result = recv(sock, buffer.data(), buffer.capacity(), 0);
    if (result > 0)
    {
        // Set the buffer size to the number of bytes read
        buffer.resize(result);
    }
    else if (result == 0)
    {
        // Connection has been gracefully closed
        close();
    }
    else if (!isClosed())
    {
        // Socket read error; socket is still open on our side but may have been closed by the other side
        std::cerr << "Failed to read from socket: " << std::to_string(WSAGetLastError()) << "\n";
        close();
    }
}

}  // namespace Rival

#endif
