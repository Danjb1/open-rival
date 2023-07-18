#include "utils/LogUtils.h"

#include "spdlog/sinks/basic_file_sink.h"

namespace Rival { namespace LogUtils {

std::string getLogFilename()
{
    const auto now = std::chrono::system_clock::now();
    std::ostringstream ss;
    // See: https://en.cppreference.com/w/cpp/chrono/system_clock/formatter#Format_specification
    ss << "logs/Open-Rival_"                        //
       << std::format("{:%Y-%m-%d_%H-%M-%S}", now)  //
       << ".log";
    return ss.str();
}

void initLogging(const std::string& logLevel, bool logToFile)
{
    if (logToFile)
    {
        try
        {
            const std::string logFilename = getLogFilename();
            auto file_logger = spdlog::basic_logger_mt("file_logger", logFilename);
            spdlog::set_default_logger(file_logger);
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            LOG_WARN("Failed to create log file: {}", ex.what());
        }
    }

    // Set log pattern: https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    // Example:
    //   [17:26:36:611] [debug] Message
    spdlog::set_pattern("[%H:%M:%S:%e] [%l] %v");

    spdlog::set_level(spdlog::level::from_str(logLevel));
}

}}  // namespace Rival::LogUtils
