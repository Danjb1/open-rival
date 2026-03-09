#include "main/ProgramOptions.h"

#include <limits>
#include <stdexcept>

#include "game/PlayerState.h"

namespace Rival {

ProgramOptions::ProgramOptions(int argc, char* argv[])
{
    error = parseArgs(argc, argv);
}

bool ProgramOptions::hasWindowPosition() const
{
    return windowPos.x > 0 && windowPos.y > 0;
}

const std::string ProgramOptions::parseArgs(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // host
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (arg == "-host")
        {
            host = true;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // connect
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        else if (arg == "-connect")
        {
            try
            {
                hostAddress = parseString(argc, argv, i + 1);
                ++i;  // Skip next argument
            }
            catch (const std::runtime_error& e)
            {
                return std::string(e.what()) + "\nExpected: -connect [hostAddress]";
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // port
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        else if (arg == "-port")
        {
            try
            {
                port = parseUint16(argc, argv, i + 1);
                ++i;  // Skip next argument
            }
            catch (const std::runtime_error& e)
            {
                return std::string(e.what()) + "\nExpected: -port [port]";
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // pos
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        else if (arg == "-pos")
        {
            try
            {
                windowPos = parseVec2i(argc, argv, i + 1);
                ++i;  // Skip next argument
            }
            catch (const std::runtime_error& e)
            {
                return std::string(e.what()) + "\nExpected: -pos [x,y]";
            }
        }

        else
        {
            return "Invalid argument: " + arg;
        }
    }

    // Final validation
    if (host && !hostAddress.empty())
    {
        return "connect argument not valid when hosting";
    }

    return {};
}

bool ProgramOptions::hasError() const
{
    return !error.empty();
}

int ProgramOptions::parseInt(int argc, char* argv[], int index, int min, int max) const
{
    if (index >= argc)
    {
        throw std::runtime_error("No value supplied for argument");
    }
    try
    {
        int val = std::stoi(argv[index]);
        if (val < min || val > max)
        {
            throw std::runtime_error("Argument out of range");
        }
        return val;
    }
    catch (const std::invalid_argument&)
    {
        throw std::runtime_error("Invalid value supplied for integer argument");
    }
    catch (const std::out_of_range&)
    {
        throw std::runtime_error("Value must be between "  //
                + std::to_string(min) + " and "            //
                + std::to_string(max));
    }
}

std::uint16_t ProgramOptions::parseUint16(int argc, char* argv[], int index) const
{
    return static_cast<std::uint16_t>(parseInt(argc, argv, index, 0, std::numeric_limits<std::uint8_t>::max()));
}

std::string ProgramOptions::parseString(int argc, char* argv[], int index) const
{
    if (index >= argc)
    {
        throw std::runtime_error("No value supplied for argument");
    }
    return argv[index];
}

glm::ivec2 ProgramOptions::parseVec2i(int argc, char* argv[], int index) const
{
    if (index >= argc)
    {
        throw std::runtime_error("No value supplied for argument");
    }

    const std::string argStr = argv[index];

    const auto commaPos = argStr.find(',');
    if (commaPos == std::string::npos)
    {
        throw std::runtime_error("No comma found in vector argument");
    }

    const std::string xStr = argStr.substr(0, commaPos);
    const std::string yStr = argStr.substr(commaPos + 1);

    try
    {
        int x = std::stoi(xStr);
        int y = std::stoi(yStr);
        return { x, y };
    }
    catch (const std::invalid_argument&)
    {
        throw std::runtime_error("Invalid integer value in vector argument");
    }
}

}  // namespace Rival
