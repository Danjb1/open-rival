#pragma once

#include <cstdint>
#include <string>

namespace Rival {

class ProgramOptions
{
public:
    ProgramOptions(int argc, char* argv[]);

    bool hasError() const;

    const std::string& getError() const
    {
        return error;
    }

    bool isHost() const
    {
        return host;
    }

    bool isClient() const
    {
        return !hostAddress.empty();
    }

    bool isNetworked() const
    {
        return isHost() || isClient();
    }

    std::string getHostAddress() const
    {
        return hostAddress;
    }

    std::uint16_t getPort() const
    {
        return port;
    }

private:
    const std::string parseArgs(int argc, char* argv[]);
    int parseInt(int argc, char* argv[], int index, int min, int max) const;
    std::uint16_t parseUint16(int argc, char* argv[], int index) const;
    std::string parseString(int argc, char* argv[], int index) const;

private:
    std::string error;

    // Default options
    bool host = false;
    std::string hostAddress;
    uint16_t port = 25565;
};

}  // namespace Rival
