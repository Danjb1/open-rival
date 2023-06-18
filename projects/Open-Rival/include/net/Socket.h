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

/**
 * Wrapper class for a native socket.
 *
 * Implementation details are platform-specific.
 */
class Socket
{
public:
    /** Creates a listening socket connected to localhost. */
    static Socket createServer(std::uint16_t port);

    /** Creates a socket and attempts to connect it to the given address and port. */
    static Socket createClient(const std::string& address, std::uint16_t port);

    Socket();
    Socket(SOCKET handle);
    ~Socket();

    bool operator==(const Socket& other) const;
    bool operator!=(const Socket& other) const;

    // Allow moving but prevent copying
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) noexcept;
    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) noexcept;

    /** Blocking call that waits for a new connection. */
    Socket accept();

    /** Closes the socket; forces any blocking calls to return. */
    void close() noexcept;

    /** Determines if this socket is currently open. */
    bool isOpen() const;

    /** Sends data on this socket. */
    void send(const std::vector<char>& buffer);

    /** Blocking call that waits for data to arrive and adds it to the given buffer. */
    void receive(std::vector<char>& buffer);

private:
    Socket(int domain, int type, int protocol);

    void init();

private:
    SOCKET handle;
};

}  // namespace Rival
