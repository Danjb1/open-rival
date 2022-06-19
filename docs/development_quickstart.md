# Development Quickstart

These are the steps necessary in order to build and run the project.

Note that the project is set up for **32-bit x86 platforms**.

## Dependencies

### Summary

#### [FreeType](https://www.freetype.org/download.html)

- **Used for:** Font rendering
- *Requires Visual C++ 2015-2019 Redistributable*
- [Install guide](https://github.com/ubawurinna/freetype-windows-binaries)

#### [glew](http://glew.sourceforge.net/):

- **Used for:** OpenGL functionality (rendering)
- [Install guide](http://lazyfoo.net/tutorials/SDL/51_SDL_and_modern_opengl/index.php)

#### [GLM](https://github.com/g-truc/glm)

- **Used for:** Matrix operations
- [Install guide](https://github.com/g-truc/glm/blob/master/manual.md#-1-getting-started)

#### [json](https://github.com/nlohmann/json)

- **Used for:** JSON parsing

#### [OpenAL Soft](https://github.com/kcat/openal-soft)

- **Used for:** OpenAL functionality (playing sounds)

#### [RtMidi](https://github.com/thestk/rtmidi)

- **Used for:** MIDI playback
- Needs to be built from source to create `RtMidi.lib`
    - A Visual Studio project is [supplied](Open-Rival/libs/rtmidi-5.0.0) for this purpose
    - See [this guide](https://www.music.mcgill.ca/~gary/rtmidi/index.html#compiling) for more information on compiling the library
        - On Windows, this requires the `__WINDOWS_MM__` preprocessor definition and the `winmm.lib` library
- On Windows, this uses the built-in [Windows Multimedia API](https://docs.microsoft.com/en-us/windows/win32/multimedia/windows-multimedia-start-page)

#### [SDL2](https://www.libsdl.org/download-2.0.php)

- **Used for:** Window creation and input
- [Install guide](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php)

#### [SDL_image](https://www.libsdl.org/projects/SDL_image/)

- **Used for:** PNG image loading
- [Install guide](https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/windows/msvc2019/index.php)

### Directory Structure

These dependencies should be placed in the `libs` folder like so:

```
+-- Open-Rival/
    +-- libs/
        +-- freetype-2.10.4/
        +-- glew-2.1.0/
        +-- glm-0.9.9.5/
        +-- json/
            +-- nlohmann/
        +-- openal-soft-1.21.0/
        +-- rtmidi-5.0.0/
        +-- SDL2-2.0.14/
        +-- SDL2_image-2.0.5/
```

### Project Configuration

The supplied Visual Studio project is already configured with these dependencies, but the required settings are included here for reference.

#### Include Directories

```
libs/SDL2-2.0.14/include
libs/SDL2_image-2.0.5/include
libs/freetype-2.10.4/include
libs/glew-2.1.0/include
libs/glm-0.9.9.5
libs/json
libs/openal-soft-1.21.0/include
libs/rtmidi-5.0.0
```

#### Library Directories

```
libs/SDL2-2.0.14/lib/x86
libs/SDL2_image-2.0.5/lib/x86
libs/freetype-2.10.4/release dll/win32
libs/glew-2.1.0/lib/Release/Win32
libs/openal-soft-1.21.0/libs/Win32
libs/rtmidi-5.0.0/{Debug|Release}
```

#### Linker Additional Dependencies

```
freetype.lib
glew32.lib
glu32.lib
OpenAL32.lib
opengl32.lib
RtMidi.lib
SDL2.lib
SDL2main.lib
SDL2_image.lib
winmm.lib
```

### DLL Files

These should be placed in the project directory (`Open-Rival`):

```
freetype.dll
glew32.dll
libpng16-16.dll (required by SDL2_image.dll)
OpenAL32.dll (renamed from soft_oal.dll)
SDL2.dll
SDL2_image.dll
zlib1.dll (required by SDL2_image.dll)
```

## Build

After installing the above dependencies, build the solution using Microsoft Visual Studio (tested with Community Edition 2022).

## Extract Resources

For legal reasons, Open-Rival does not distribute any of the original game's resources. However, a setup utility is provided that is capable of extracting the required resources from the original game.

The [setup project](/setup) can be trivially built using Visual Studio. Once built, run the "setup" project to perform resource extraction (instructions in the [readme](/setup/README.md)). This should create the folders `sound`, `textures` and `video` within the `setup/res` directory.

Copy these folders to the `Open-Rival/res` directory.

## Install Font

The game uses the [Procopius font](https://fontsgeek.com/fonts/Procopius-Regular) which must be downloaded and installed separately.

## Run

Once the dependencies and resources are in place, you should be able to run the "Open-Rival" project!

## Release

Instructions on building and packaging a version for release can be found [here](/docs/release_checklist.md).
