#include "pch.h"

#include "net/Connection.h"

namespace Rival {

Connection::Connection(std::shared_ptr<Socket> destination)
    : destination(destination)
{
}

void Connection::close()
{
    destination->close();
}

}  // namespace Rival
