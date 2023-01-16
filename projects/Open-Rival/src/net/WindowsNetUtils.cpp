#include "pch.h"

#ifdef _WIN32

#include <winsock2.h>

#include <stdexcept>

namespace Rival { namespace NetUtils {

void initNetworking()
{
    WSADATA wsaData;

    if (int result = WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        throw std::runtime_error("Failed to initialize Winsock: " + result);
    }
}

}}  // namespace Rival::NetUtils

#endif
