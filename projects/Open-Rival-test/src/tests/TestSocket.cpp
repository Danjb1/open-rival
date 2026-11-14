#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <future>
#include <thread>
#include <vector>

#include "net/Connection.h"
#include "net/NetUtils.h"
#include "net/PacketFactory.h"
#include "net/Socket.h"

using namespace Rival;

namespace TestSocket {

using namespace std::chrono_literals;

/** How long to wait before shutting down a socket that another thread is blocked on. */
constexpr auto delayBeforeShutdown = 100ms;

/** How long to wait for a blocked call to return. If it takes longer than this, we consider it stuck. */
constexpr auto timeout = 5s;

/** Initializes networking for as long as it is in scope. */
struct NetworkingGuard
{
    NetworkingGuard()
    {
        NetUtils::initNetworking();
    }

    ~NetworkingGuard()
    {
        NetUtils::destroyNetworking();
    }
};

SCENARIO("Sockets can send and receive data", "[net][socket]")
{
    NetworkingGuard networking;

    GIVEN("A client connected to a server")
    {
        // Port 0 picks any available port
        Socket server = Socket::createServer(0);
        Socket client = Socket::createClient("localhost", server.getLocalPort());
        Socket serverSide = server.accept();
        REQUIRE(serverSide.isOpen());

        WHEN("the client sends some data")
        {
            const std::vector<char> sentData = { 'a', 'b', 'c', 'd' };
            client.send(sentData);

            THEN("the server receives it")
            {
                std::vector<char> receivedData(sentData.size());
                serverSide.receive(receivedData);
                REQUIRE(receivedData == sentData);
                REQUIRE(serverSide.isOpen());
            }
        }

        WHEN("the client shuts down")
        {
            client.shutdown();

            THEN("the server stops receiving")
            {
                std::vector<char> buffer(4);
                serverSide.receive(buffer);
                REQUIRE(!serverSide.isOpen());
            }
        }
    }
}

SCENARIO("Shutting down a socket unblocks other threads", "[net][socket]")
{
    NetworkingGuard networking;

    GIVEN("A thread waiting to receive data")
    {
        Socket server = Socket::createServer(0);
        Socket client = Socket::createClient("localhost", server.getLocalPort());
        Socket serverSide = server.accept();
        REQUIRE(serverSide.isOpen());

        auto receiveTask = std::async(std::launch::async, [&]() {
            std::vector<char> buffer(4);
            serverSide.receive(buffer);
        });

        WHEN("the socket is shut down by another thread")
        {
            std::this_thread::sleep_for(delayBeforeShutdown);
            serverSide.shutdown();

            THEN("the receiving thread stops waiting")
            {
                const bool finished = receiveTask.wait_for(timeout) == std::future_status::ready;
                if (!finished)
                {
                    // Unblock the thread so that the test can finish
                    client.close();
                    receiveTask.wait();
                }
                REQUIRE(finished);
                REQUIRE(!serverSide.isOpen());
            }
        }
    }

    GIVEN("A thread waiting to accept a connection")
    {
        Socket server = Socket::createServer(0);

        auto acceptTask = std::async(std::launch::async, [&]() { return server.accept(); });

        WHEN("the socket is shut down by another thread")
        {
            std::this_thread::sleep_for(delayBeforeShutdown);
            server.shutdown();

            THEN("the accepting thread stops waiting, without accepting a connection")
            {
                REQUIRE(acceptTask.wait_for(timeout) == std::future_status::ready);
                REQUIRE(!acceptTask.get().isOpen());
                REQUIRE(!server.isOpen());
            }
        }
    }
}

SCENARIO("A Connection can be closed while the other side is silent", "[net][socket]")
{
    NetworkingGuard networking;

    GIVEN("A Connection whose receive thread is waiting for data that never arrives")
    {
        Socket server = Socket::createServer(0);
        Connection connection(
                Socket::createClient("localhost", server.getLocalPort()), std::make_shared<PacketFactory>());
        Socket serverSide = server.accept();
        std::this_thread::sleep_for(delayBeforeShutdown);

        WHEN("the Connection is closed")
        {
            auto closeTask = std::async(std::launch::async, [&]() { connection.close(); });

            THEN("it closes without waiting for the other side")
            {
                const bool finished = closeTask.wait_for(timeout) == std::future_status::ready;
                if (!finished)
                {
                    // Unblock the Connection's threads so that the test can finish
                    serverSide.close();
                    closeTask.wait();
                }
                REQUIRE(finished);
                REQUIRE(!connection.isOpen());
            }
        }
    }
}

}  // namespace TestSocket
