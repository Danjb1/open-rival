# Dependencies

## Overview

### [FreeType](https://www.freetype.org/download.html)

> **Used for:** Font rendering

- Dynamically-linked.
- Needs to be built from source to create `freetype.lib`.
    - A Visual Studio project is included in the download (`builds/windows/vc2010`).
    - "Debug" and "Release" configurations are provided.

### [glew](http://glew.sourceforge.net/)

> **Used for:** OpenGL functionality (rendering)

[Install guide](https://glew.sourceforge.net/install.html)

- Statically-linked (as recommended [here](https://stackoverflow.com/a/20873711/1624459)).
- Needs to be built from source to create `glew32sd.lib` / `glew32s.lib` (Debug / Release).
    - A Visual Studio project is included in the download (`build/vc12`).
    - Clear the Runtime Library property (C/C++ > Code Generation) to avoid linker warnings.

### [GLM](https://github.com/g-truc/glm)

> **Used for:** Matrix operations

[Install guide](https://github.com/g-truc/glm/blob/master/manual.md#-1-getting-started)

- Header-only library.

### [json](https://github.com/nlohmann/json)

> **Used for:** JSON parsing

- Header-only library (single header).

### [OpenAL Soft](https://github.com/kcat/openal-soft)

> **Used for:** OpenAL functionality (playing sounds)

- Dynamically-linked.
- Needs to be built from source to create `OpenAL32.lib` (yes, even for 64-bit).
    - A Visual Studio solution must be generated using CMake.

### [RtMidi](https://github.com/thestk/rtmidi)

> **Used for:** MIDI playback

- Dynamically-linked.
- Needs to be built from source to create `rtmidi.lib`.
    - A Visual Studio solution must be generated using CMake.

### [SDL2](https://github.com/libsdl-org/SDL)

> **Used for:** Window creation and input

[Install guide](https://wiki.libsdl.org/SDL2/Installation)

- Dynamically-linked (as recommended by the [library authors](https://wiki.libsdl.org/SDL2/Installation#static_linking)).
- On Windows, Debug and Release versions use [the same `.lib` file](https://wiki.libsdl.org/SDL2/Installation#windows_xpvista7810)!

### [SDL_image](https://github.com/libsdl-org/SDL_image)

> **Used for:** PNG image loading

- Dynamically-linked.
[Install guide](https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/windows/msvc2019/index.php)

### [spdlog](https://github.com/gabime/spdlog)

> **Used for:** Logging

- Statically-linked.
- Needs to be built from source to create `spdlogd.lib` / `spdlog.lib` (Debug / Release).

### [Windows Multimedia API](https://docs.microsoft.com/en-us/windows/win32/multimedia/windows-multimedia-start-page)

> **Used for:** High-precision timers, MIDI playback (Windows only)

- This library is included by default on Windows machines.

### [Winsock](https://learn.microsoft.com/en-us/windows/win32/winsock/about-winsock)

> **Used for:** Networking (Windows only)

- This library is included by default on Windows machines.

## DLL Files

```
freetype.dll
OpenAL32.dll
rtmidi.dll
SDL2.dll
SDL2_image.dll
```
