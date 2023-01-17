#pragma once

#include <memory>

#include "net/Socket.h"

namespace Rival {

/** Manages a connection to a socket. */
class Connection
{
public:
    Connection(std::shared_ptr<Socket> destination);

    /** Closes this connection. */
    void close();

private:
    std::shared_ptr<Socket> destination;
};

}  // namespace Rival
