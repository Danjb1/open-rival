#pragma once

#include <memory>

#include "net/Socket.h"

namespace Rival {

/** Manages a connection to a socket. */
class Connection
{
public:
    Connection(Socket destination);

    /** Closes this connection. */
    void close();

private:
    Socket destination;
};

}  // namespace Rival
