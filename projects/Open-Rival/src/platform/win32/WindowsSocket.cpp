#include "pch.h"

#ifdef _WIN32

// These comments...
#include "net/Socket.h"
// ... prevent the auto-formatter from moving the include

#include <winsock2.h>

#include <ws2tcpip.h>

#include <stdexcept>

namespace Rival {

Socket Socket::createServer(int port)
{
    return Socket("localhost", port, true);
}

Socket Socket::createClient(const std::string& address, int port)
{
    return Socket(address, port, false);
}

Socket::Socket(const std::string& address, int port, bool server)
{
    // Specify socket properties
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    addrinfo* addrInfo = nullptr;
    if (int err = getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &addrInfo))
    {
        throw std::runtime_error("Failed to get net address: " + err);
    }

    // Create the socket
    sock = INVALID_SOCKET;
    sock = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);

    // Check for errors
    if (sock == INVALID_SOCKET)
    {
        int err = WSAGetLastError();
        freeaddrinfo(addrInfo);
        throw std::runtime_error("Failed to create socket: " + err);
    }

    // Bind the socket
    int bindResult = bind(sock, addrInfo->ai_addr, (int) addrInfo->ai_addrlen);
    if (bindResult == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        freeaddrinfo(addrInfo);
        throw std::runtime_error("Failed to bind socket: " + err);
    }

    // Address info is no longer needed
    freeaddrinfo(addrInfo);

    // Listen (server-only)
    if (server && listen(sock, SOMAXCONN) == SOCKET_ERROR)
    {
        throw std::runtime_error("Failed to listen on socket: " + WSAGetLastError());
    }
}

Socket::~Socket()
{
    closesocket(sock);
}

}  // namespace Rival

#endif
