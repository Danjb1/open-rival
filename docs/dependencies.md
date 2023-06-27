# Dependencies

## Overview

### [FreeType](https://www.freetype.org/download.html)

> **Used for:** Font rendering

- Needs to be built from source to create `freetype.lib`.
    - A Visual Studio project is included in the download (`builds/windows/vc2010`).
    - "Debug Static" and "Release Static" configurations are provided.

### [glew](http://glew.sourceforge.net/)

> **Used for:** OpenGL functionality (rendering)

[Install guide](http://lazyfoo.net/tutorials/SDL/51_SDL_and_modern_opengl/index.php)

- Static linking [recommended](https://stackoverflow.com/a/20873711/1624459).
- Needs to be built from source to create `glew32sd.lib` / `glew32s.lib` (Debug / Release).
    - A Visual Studio project is included in the download (`build/vc12`).

### [GLM](https://github.com/g-truc/glm)

> **Used for:** Matrix operations

[Install guide](https://github.com/g-truc/glm/blob/master/manual.md#-1-getting-started)

- Header-only library.

### [json](https://github.com/nlohmann/json)

> **Used for:** JSON parsing

- Header-only library (single header).

### [OpenAL Soft](https://github.com/kcat/openal-soft)

> **Used for:** OpenAL functionality (playing sounds)

- Needs to be built from source to create `OpenAL32.lib`.

### [RtMidi](https://github.com/thestk/rtmidi)

> **Used for:** MIDI playback

- Needs to be built from source to create `rtmidi.lib`.
    - A Visual Studio solution must be generated using CMake.

### [SDL2](https://github.com/libsdl-org/SDL)

> **Used for:** Window creation and input

[Install guide](https://wiki.libsdl.org/SDL2/Installation)

- Dynamic linking recommended by the library authors.
- On Windows, Debug and Release versions use [the same `.lib` file](https://wiki.libsdl.org/SDL2/Installation#windows_xpvista7810)!

### [SDL_image](https://github.com/libsdl-org/SDL_image)

> **Used for:** PNG image loading

[Install guide](https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/windows/msvc2019/index.php)

### [Windows Multimedia API](https://docs.microsoft.com/en-us/windows/win32/multimedia/windows-multimedia-start-page)

> **Used for:** High-precision timers, MIDI playback (Windows only)

- This library is included by default on Windows machines.

### [Winsock](https://learn.microsoft.com/en-us/windows/win32/winsock/about-winsock)

> **Used for:** Networking (Windows only)

- This library is included by default on Windows machines.

## Example Directory Structure

```
+-- Open-Rival/
    +-- libs/
        +-- freetype-2.13.0/
            +-- include/
            +-- objs/
                +-- x64/
                    +-- Debug/
                        +-- freetype.lib
                    +-- Release/
                        +-- freetype.lib
        +-- glew-2.1.0/
            +-- bin/
                +-- Debug/
                    +-- x64/
                        +-- glew32d.lib
                +-- Release/
                    +-- x64/
                        +-- glew32.lib
            +-- include/
        +-- glm-0.9.9.8/
            +-- glm/
        +-- json-3.11.2/
            +-- include/
                +-- nlohmann/
                    +-- json.hpp
        +-- openal-soft-1.23.1
            +-- build/
                +-- Debug/
                    +-- OpenAL32.dll
                +-- Release/
                    +-- OpenAL32.dll
            +-- include/
        +-- rtmidi-5.0.0
            +-- build/
                +-- Debug/
                    +-- rtmidi.lib
                +-- Release/
                    +-- rtmidi.lib
        +-- SDL2_image-2.6.3/
            +-- include/
            +-- lib/
                +-- x64/
                    +-- SDL2_image.lib
        +-- SDL2-2.28.0/
            +-- include/
            +-- lib/
                +-- x64/
                    +-- SDL2.lib
```

## DLL Files

```
freetype.dll
libpng16-16.dll     (required by SDL2_image.dll)
OpenAL32.dll
rtmidi.dll
SDL2.dll
SDL2_image.dll
zlib1.dll           (required by SDL2_image.dll)
```
