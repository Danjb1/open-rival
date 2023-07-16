#pragma once

#include <spdlog/spdlog.h>

#include <string.h>

// Wrapper macros around library functions.
// This enables us to switch logging libraries easily if desired.
// Formatting guide: https://fmt.dev/latest/syntax.html
#define LOG_TRACE(...) spdlog::trace(__VA_ARGS__)
#define LOG_DEBUG(...) spdlog::debug(__VA_ARGS__)
#define LOG_INFO(...) spdlog::info(__VA_ARGS__)
#define LOG_WARN(...) spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
#define LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)

namespace Rival { namespace LogUtils {

void initLogging(const std::string& logLevel);

}}  // namespace Rival::LogUtils
