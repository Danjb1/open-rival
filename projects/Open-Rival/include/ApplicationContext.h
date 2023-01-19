#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "AudioSystem.h"
#include "JsonUtils.h"
#include "Resources.h"
#include "Window.h"

namespace Rival {

/**
 * Manages resources that are vital to the running of the application.
 *
 * If the ApplicationContext initializes successfully, then everything else should work fine!
 */
class ApplicationContext
{
public:
    ApplicationContext();
    ~ApplicationContext();

    Window* getWindow()
    {
        return window.get();
    }

    json& getConfig()
    {
        return cfg;
    }

    AudioSystem& getAudioSystem()
    {
        return audioSystem;
    }

    Resources& getResources()
    {
        return res;
    }

    FT_Library& getFontLibrary()
    {
        return fontLib;
    }

private:
    static json readConfig();
    static void initGLEW();
    static void initGL();
    static std::unique_ptr<Window> createWindow();
    static void setWindowIcon(Window& window);

    void initSDL();
    void initAudio();
    void initFonts();

private:
    json cfg;
    std::unique_ptr<Window> window;
    AudioSystem audioSystem;
    Resources res;
    FT_Library fontLib;
};

}  // namespace Rival
