#pragma once

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

    int getPort() const
    {
        return port;
    }

private:
    const std::string parseArgs(int argc, char* argv[]);
    int parseInt(int argc, char* argv[], int index, int min, int max) const;
    std::string parseString(int argc, char* argv[], int index) const;

private:
    std::string error;

    // Default options
    bool host = false;
    std::string hostAddress;
    int port = 25565;
};

}  // namespace Rival
