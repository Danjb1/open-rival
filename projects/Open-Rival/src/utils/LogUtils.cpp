#include "utils/LogUtils.h"

#include "spdlog/sinks/basic_file_sink.h"

namespace Rival { namespace LogUtils {

void initLogging(const std::string& logLevel)
{
    try
    {
        // Log to file
        const auto now = std::chrono::system_clock::now();
        std::ostringstream ss;
        // See: https://en.cppreference.com/w/cpp/chrono/system_clock/formatter#Format_specification
        ss << "logs/Open-Rival_"                        //
           << std::format("{:%Y-%m-%d_%H-%M-%S}", now)  //
           << ".log";
        const std::string logFilename = ss.str();
        auto file_logger = spdlog::basic_logger_mt("file_logger", logFilename);
        spdlog::set_default_logger(file_logger);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Log init failed: " << ex.what() << "\n";
    }
    spdlog::set_level(spdlog::level::from_str(logLevel));
}

}}  // namespace Rival::LogUtils
