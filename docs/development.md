# Development Quickstart

These are the steps necessary in order to build and run the project.

Note that the project is designed to run on **x86 platforms**.

## Install Dependencies

These tutorials describe how to install each of our dependencies:

 - [glew](http://lazyfoo.net/tutorials/SDL/51_SDL_and_modern_opengl/index.php)

 - [SDL2](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php)

 - [GLM](https://github.com/g-truc/glm/blob/master/manual.md#-1-getting-started)

### Directory Structure

These dependencies should be placed in the `libs` folder:

    +-- Open-Rival/
        +-- libs/
            +-- glew-2.1.0/
            +-- SDL2-2.0.8/

### Include Directories

    libs/SDL2-2.0.8/include
    libs/glew-2.1.0/include
    libs/glm-0.9.9.5

### Library Directories

    libs/SDL2-2.0.8/lib/x86
    libs/glew-2.1.0/lib/Release/Win32

### Linker Additional Dependencies

    glew32.lib
    glu32.lib
    opengl32.lib
    SDL2.lib
    SDL2main.lib

### DLL Files

These should be placed in the project directory:

    glew32.dll
    SDL2.dll

## Build

After installing the above dependencies, build the solution using Microsoft Visual Studio 2017 or later.

## Extract Resources

For legal reasons, Open-Rival does not distribute any of the original game's resources. However, a setup utility is provided that is capable of extracting the required resources from the original game, provided it has been installed correctly.

> Specifically, the game looks for the registry key `HKEY_CURRENT_USER\Software\Titus Games\Rival Realms\Game Directory`. A suitable registry file is included with [this download](https://www.old-games.com/download/5514/rival-realms) of the original game, or can be easily created.

Once built, run the "setup" project to perform resource extraction. This should create the folders `sound`, `textures` and `video` within the `setup/res` directory.

Copy these folders to the `Open-Rival/res` directory.

## Run

Once the dependencies and resources are in place, you should be able to run the "Open-Rival" project!
