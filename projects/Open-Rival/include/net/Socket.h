#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

// Ideally we would not have any platform-specific stuff here,
// but this seems unavoidable since we need to know the underlying type!
#ifdef _WIN32
typedef unsigned long long SOCKET;
#else
typedef int SOCKET;
#endif

// Borrowed from WinSock but should also work for Unix (~0 is equal to -1)
#ifndef INVALID_SOCKET
#define INVALID_SOCKET (SOCKET)(~0)
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

    /** Blocking call that waits for a new connection.
     * Returns an invalid Socket if this socket is shut down while waiting. */
    Socket accept();

    /**
     * Stops all communication on this socket, and forces any blocking calls to return.
     *
     * This is safe to call from any thread. The socket is not released until `close()` is called.
     */
    void shutdown() noexcept;

    /**
     * Releases the socket.
     *
     * This must only be called when no other thread is using the socket. To stop other threads from using the socket
     * (e.g. to unblock a call to `receive`), call `shutdown()` first, and wait for those threads to finish.
     */
    void close() noexcept;

    /** Determines if this socket is currently open, i.e. it has not been shut down or closed. */
    bool isOpen() const;

    /** Gets the local port to which this socket is bound, or 0 if this cannot be determined.
     * This is useful when creating a server on port 0, which picks any available port. */
    std::uint16_t getLocalPort() const;

    /** Sends data on this socket. */
    void send(const std::vector<char>& buffer);

    /** Blocking call that waits for data to arrive and adds it to the given buffer. */
    void receive(std::vector<char>& buffer);

private:
    Socket(int domain, int type, int protocol);

    void init();

    /** Waits until this socket is ready to be read from (or written to).
     * Returns false if the socket is shut down while waiting, or an error occurs. */
    bool waitUntilReady(bool forWriting) const;

private:
    SOCKET handle;

    /** Atomic because a socket may be shut down by one thread while another thread is using it. */
    std::atomic<bool> isShutDown = false;
};

}  // namespace Rival
