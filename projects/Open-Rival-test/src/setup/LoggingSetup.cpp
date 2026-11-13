#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <nlohmann/json.hpp>

#include "utils/LogUtils.h"

using namespace Rival;

/**
 * Registers the log categories used by the game before any tests run.
 *
 * The game does this at startup (see Main.cpp); without it, any code that logs to a category would crash, since
 * spdlog::get returns null for unregistered loggers.
 */
class LoggingSetupListener : public Catch::EventListenerBase
{
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(const Catch::TestRunInfo&) override
    {
        // No config, so every category uses the default level.
        // Only show warnings and above, to keep the test output readable.
        LogUtils::makeLogCategories(nlohmann::json(), "warn");
    }
};

CATCH_REGISTER_LISTENER(LoggingSetupListener)
