#ifdef _WIN32

// These comments...
#include "net/NetUtils.h"
// ... prevent the auto-formatter from moving the include

#include <winsock2.h>

#include <stdexcept>

namespace Rival { namespace NetUtils {

void initNetworking()
{
    WSADATA wsaData;

    if (int err = WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        throw std::runtime_error("Failed to initialize Winsock: " + err);
    }
}

void destroyNetworking()
{
    WSACleanup();
}

}}  // namespace Rival::NetUtils

#endif
