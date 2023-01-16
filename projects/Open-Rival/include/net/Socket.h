#pragma once

#include <string>

// Ideally we would not have any platform-specific stuff here,
// but this seems unavoidable since we need to know the underlying type!
#ifdef _WIN32
#include <winsock2.h>
#else
typedef int SOCKET;
#endif

namespace Rival {

class Socket
{
private:
    Socket(const std::string& address, int port, bool server);

public:
    static Socket createServer(int port);
    static Socket createClient(const std::string& address, int port);
    ~Socket();

private:
    SOCKET sock;
};

}  // namespace Rival
