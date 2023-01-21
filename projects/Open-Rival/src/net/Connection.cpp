#include "pch.h"

#include "net/Connection.h"

namespace Rival {

Connection::Connection(Socket destination)
    : destination(std::move(destination))
{
}

void Connection::close()
{
    destination.close();
}

}  // namespace Rival
