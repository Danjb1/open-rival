#ifdef _WIN32

/* clang-format off */
#include "net/NetUtils.h"
/* clang-format on */

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
