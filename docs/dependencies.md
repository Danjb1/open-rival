# Dependencies

## Overview

### [FreeType](https://www.freetype.org/download.html)

> **Used for:** Font rendering.

### [glew](http://glew.sourceforge.net/)

> **Used for:** Determining supported OpenGL extensions.

### [GLM](https://github.com/g-truc/glm)

> **Used for:** Matrix operations.

### [json](https://github.com/nlohmann/json)

> **Used for:** JSON parsing.

### [OpenAL Soft](https://github.com/kcat/openal-soft)

> **Used for:** OpenAL functionality (playing sounds).

### [OpenGL](https://learn.microsoft.com/en-us/windows/win32/opengl/opengl)

> **Used for:** Rendering.

- Included with Windows.

### [RtMidi](https://github.com/thestk/rtmidi)

> **Used for:** MIDI playback.

### [SDL2](https://github.com/libsdl-org/SDL)

> **Used for:** Window creation and input.

[Install guide](https://wiki.libsdl.org/SDL2/Installation)

### [SDL_image](https://github.com/libsdl-org/SDL_image)

> **Used for:** PNG image loading.

### [spdlog](https://github.com/gabime/spdlog)

> **Used for:** Logging.

### [Windows Multimedia API](https://docs.microsoft.com/en-us/windows/win32/multimedia/windows-multimedia-start-page)

> **Used for:** High-precision timers, MIDI playback (Windows only).

- Included with Windows.

### [Winsock](https://learn.microsoft.com/en-us/windows/win32/winsock/about-winsock)

> **Used for:** Networking (Windows only).

- Included with Windows.

## DLL Files

These are the DLL files that should be shipped with the game. They can all be found in the output directory after building.

```
OpenAL32.dll
SDL2_image.dll
SDL2.dll
brotlicommon.dll
brotlidec.dll
bz2.dll
fmt.dll
freetype.dll
glew32.dll
libpng16.dll
spdlog.dll
zlib1.dll
```

> Some of these are transitive dependencies. These can be identified by running, for example, `vcpkg depend-info freetype`.
