#pragma once

#include <cstdint>
#include <memory>
#include <string>

// Ideally we would not have any platform-specific stuff here,
// but this seems unavoidable since we need to know the underlying type!
#ifdef _WIN32
#include <winsock2.h>
#else
typedef int SOCKET;
#endif

namespace Rival {

enum class SocketState : std::uint8_t
{
    Open,
    Closed
};

class Socket
{
private:
    Socket(const std::string& address, int port, bool server);

public:
    static Socket createServer(int port);
    static Socket createClient(const std::string& address, int port);
    static std::shared_ptr<Socket> wrap(SOCKET rawSocket);

    Socket(SOCKET rawSocket);
    ~Socket();

    // Disable moving / copying
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) = delete;
    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) = delete;

    /** Blocking call that waits for a new connection. */
    std::shared_ptr<Socket> accept();

    /** Closes the socket; forces any blocking calls to return. */
    void close();

    bool isClosed() const;

private:
    void init();

private:
    SOCKET sock;
    SocketState state = SocketState::Open;
};

}  // namespace Rival
