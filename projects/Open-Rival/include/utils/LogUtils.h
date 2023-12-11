#pragma once

#pragma warning(push)
#pragma warning(disable : 6285 26437 26498 26800)
#include <spdlog/spdlog.h>
#pragma warning(pop)

#include <string>

// Wrapper macros around library functions.
// This helps keep code library-agnostic.
// Formatting guide: https://fmt.dev/latest/syntax.html
#define LOG_TRACE(...) spdlog::trace(__VA_ARGS__)
#define LOG_DEBUG(...) spdlog::debug(__VA_ARGS__)
#define LOG_INFO(...) spdlog::info(__VA_ARGS__)
#define LOG_WARN(...) spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
#define LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)

// Category-based log functions
#define LOG_TRACE_CATEGORY(category, ...) spdlog::get(category)->trace(__VA_ARGS__)
#define LOG_DEBUG_CATEGORY(category, ...) spdlog::get(category)->debug(__VA_ARGS__)
#define LOG_INFO_CATEGORY(category, ...) spdlog::get(category)->info(__VA_ARGS__)
#define LOG_WARN_CATEGORY(category, ...) spdlog::get(category)->warn(__VA_ARGS__)
#define LOG_ERROR_CATEGORY(category, ...) spdlog::get(category)->error(__VA_ARGS__)
#define LOG_CRITICAL_CATEGORY(category, ...) spdlog::get(category)->critical(__VA_ARGS__)

namespace Rival { namespace LogUtils {

using Logger = std::shared_ptr<spdlog::logger>;

void initLogging(const std::string& logLevel, bool logToFile);

/** Creates a new log "category" that can have its own independent log level.
 * This should be called exactly once per category. */
Logger makeLogCategory(const std::string& category, const std::string& logLevel);

}}  // namespace Rival::LogUtils
