# Development Quickstart

These are the steps necessary in order to build and run the project.

Note that the project is set up for **32-bit x86 platforms**.

## Install Dependencies

These tutorials describe how to install each of our dependencies:

 - [glew](http://lazyfoo.net/tutorials/SDL/51_SDL_and_modern_opengl/index.php)
 - [SDL2](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php)
 - [SDL2_mixer](https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/windows/msvc2019/index.php)
 - [GLM](https://github.com/g-truc/glm/blob/master/manual.md#-1-getting-started)
 - [FreeType](https://github.com/ubawurinna/freetype-windows-binaries)

### Directory Structure

These dependencies should be placed in the `libs` folder:

    +-- Open-Rival/
        +-- libs/
            +-- freetype-2.10.4/
            +-- glew-2.1.0/
			+-- glm-0.9.9.5/
			+-- openal-soft-1.21.0/
            +-- SDL2-2.0.8/

### Include Directories

    libs/SDL2-2.0.8/include
    libs/freetype-2.10.4
    libs/glew-2.1.0/include
    libs/glm-0.9.9.5
	libs/openal-soft-1.21.0/include

### Library Directories

    libs/SDL2-2.0.8/lib/x86
    libs/freetype-2.10.4
    libs/glew-2.1.0/lib/Release/Win32
	libs/openal-soft-1.21.0/libs/Win32

### Linker Additional Dependencies

    freetype.lib
    glew32.lib
    glu32.lib
	OpenAL32.lib
    opengl32.lib
    SDL2.lib
    SDL2main.lib
    winmm.lib

### DLL Files

These should be placed in the project directory (`Open-Rival`):

    freetype.dll
    glew32.dll
	OpenAL32.dll (renamed from soft_oal.dll)
    SDL2.dll

## Build

After installing the above dependencies, build the solution using Microsoft Visual Studio (tested with Community Edition 2019).

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
