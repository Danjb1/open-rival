#include "application/ApplicationContext.h"

#include "utils/SDLWrapper.h"

#include <chrono>
#include <format>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>  // std::move

#include "application/ResourceLoader.h"
#include "application/Window.h"
#include "audio/AudioUtils.h"
#include "game/GameState.h"
#include "game/World.h"
#include "gfx/Renderer.h"
#include "net/NetUtils.h"
#include "scenario/ScenarioBuilder.h"
#include "scenario/ScenarioReader.h"
#include "utils/ConfigUtils.h"
#include "utils/JsonUtils.h"
#include "utils/LogUtils.h"

namespace Rival {

ApplicationContext::ApplicationContext(json& cfg, Window* window, Renderer* renderer)
    /* We initialize logging as early as possible, but we need to read the config first to know the log level. */
    : cfg(cfg)
    , window(window)
    , renderer(renderer)
{
    initLogging();
    initFonts();

    ResourceLoader(*this, res);  // Load resources

    initAudio();
    NetUtils::initNetworking();
}

ApplicationContext::~ApplicationContext()
{
    LOG_DEBUG("Performing clean-up...");
    NetUtils::destroyNetworking();
    FT_Done_FreeType(fontLib);
    SDL_Quit();
}

void ApplicationContext::initAudio()
{
    audioSystem.setMidiActive(ConfigUtils::get(cfg, "midiEnabled", true));
    audioSystem.setSoundActive(ConfigUtils::get(cfg, "soundEnabled", true));
    audioSystem.prepareSounds(res.getAllSounds());
}

void ApplicationContext::initFonts()
{
    if (FT_Error err = FT_Init_FreeType(&fontLib))
    {
        throw std::runtime_error("Failed to initialize FreeType library: " + std::to_string(err));
    }
}

}  // namespace Rival
