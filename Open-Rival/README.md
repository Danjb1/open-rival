# Open Rival

## Dependencies

 - **[glew](http://glew.sourceforge.net/):** For OpenGL functionality
 - **[SDL2](https://www.libsdl.org/download-2.0.php):** For window creation and input
 - **[SDL_image](https://www.libsdl.org/projects/SDL_image/)**: For PNG image loading
 - **[GLM](https://github.com/g-truc/glm)**: For matrix operations
 - **[FreeType](https://www.freetype.org/download.html)**: For font rendering
    - *Requires Visual C++ 2015-2019 Redistributable*
 - **[Windows Multimedia API](https://docs.microsoft.com/en-us/windows/win32/multimedia/windows-multimedia-start-page) (included with Windows):** For MIDI playback

## Build

Build using Visual Studio after installing the above dependencies.

See [this guide](/docs/development.md) for more details.

## Static Code Analysis

### [Cppcheck](http://cppcheck.sourceforge.net/)

This should be run periodically.

#### Quick

    cppcheck --project=Open-Rival.vcxproj "--project-configuration=Release|x64"

#### Full

    cppcheck --project=Open-Rival.vcxproj
