#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

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

/**
 * Wrapper class for a native socket.
 *
 * Implementation details are platform-specific.
 */
class Socket
{
public:
    static Socket createServer(int port);
    static Socket createClient(const std::string& address, int port);
    static Socket wrap(SOCKET rawSocket);

    ~Socket();

    bool Socket::operator==(const Socket& other) const;
    bool Socket::operator!=(const Socket& other) const;

    // Allow moving but prevent copying and move-assignment
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) noexcept;
    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) = delete;

    /** Blocking call that waits for a new connection. */
    Socket accept();

    /** Closes the socket; forces any blocking calls to return. */
    void close() noexcept;

    /** Determines if this socket is valid. */
    bool isValid() const;

    /** Determines if this socket is closed. */
    bool isClosed() const;

    /** Sends data on this socket. */
    void send(std::vector<char>& buffer);

    /** Blocking call that waits for data to arrive and adds it to the given buffer. */
    void receive(std::vector<char>& buffer);

private:
    Socket(const std::string& address, int port, bool server);
    Socket(SOCKET rawSocket);

    void init();

private:
    SOCKET sock;
    SocketState state = SocketState::Closed;
};

}  // namespace Rival
