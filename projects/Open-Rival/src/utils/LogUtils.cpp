#include "utils/LogUtils.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <iostream>
#include <memory>

namespace Rival { namespace LogUtils {

std::string getLogFilename(const std::string& filenamePrefix)
{
    const auto now = std::chrono::system_clock::now();
    std::ostringstream ss;
    // See: https://en.cppreference.com/w/cpp/chrono/system_clock/formatter#Format_specification
    ss << "logs/"                                   //
       << filenamePrefix << "_"                     //
       << std::format("{:%Y-%m-%d_%H-%M-%S}", now)  //
       << ".log";
    return ss.str();
}

void initLogging(const std::string& logLevel, bool logToConsole, bool logToFile, const std::string filenamePrefix)
{
    if (!logToConsole && !logToFile)
    {
        // No logging configured
        spdlog::default_logger()->sinks().clear();
        std::cout << "Logging is disabled!\n";
        return;
    }

    std::vector<spdlog::sink_ptr> sinkList;

    // Create console sink
    if (logToConsole)
    {
        sinkList.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    }

    // Create file sink
    if (logToFile)
    {
        try
        {
            const std::string logFilename = getLogFilename(filenamePrefix);
            sinkList.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilename));
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            LOG_WARN("Failed to create log file: {}", ex.what());
        }
    }

    // Create a logger that uses these sinks
    auto logger = std::make_shared<spdlog::logger>("default", sinkList.cbegin(), sinkList.cend());

    // Set log level
    logger->set_level(spdlog::level::from_str(logLevel));

    // Set log pattern: https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    // Example:
    //   [17:26:36:611] [debug] [category] Message
    logger->set_pattern("[%H:%M:%S:%e] [%^%l%$] [%n] %v");

    // Make this the default logger
    spdlog::set_default_logger(logger);

    // Flush all loggers when a warning or higher is logged
    spdlog::flush_on(spdlog::level::warn);
}

Logger makeLogCategory(const std::string& category, const std::string& logLevel)
{
    // Create a new logger for this category that uses the same sinks as the default logger
    Logger defaultLogger = spdlog::default_logger();
    Logger categoryLogger =
            std::make_shared<spdlog::logger>(category, defaultLogger->sinks().begin(), defaultLogger->sinks().end());

    // Set the log level for this category
    categoryLogger->set_level(spdlog::level::from_str(logLevel));

    // Globally register the logger so it can be accessed using spdlog::get(logger_name)
    spdlog::register_logger(categoryLogger);

    return categoryLogger;
}

}}  // namespace Rival::LogUtils
