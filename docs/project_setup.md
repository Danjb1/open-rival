# Project Setup

This is a guide to configuring the project with the required dependencies, for **x86 platforms**.

## Dependencies

These tutorials describe how to install each of our dependencies:

 - [glew](http://lazyfoo.net/tutorials/SDL/51_SDL_and_modern_opengl/index.php)

 - [SDL2](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php)

## Directory Structure

These dependencies should be placed in the "libs" folder:

    +-- Open-Rival/
        +-- libs/
            +-- glew-2.1.0/
            +-- SDL2-2.0.8/

## Include Directories

    libs/SDL2-2.0.8/include
    libs/glew-2.1.0/include

## Library Directories

    libs/SDL2-2.0.8/lib/x86
    libs/glew-2.1.0/lib/Release/Win32

## Linker Additional Dependencies

    glew32.lib
    glu32.lib
    opengl32.lib
    SDL2.lib
    SDL2main.lib

## DLL Files

These should be placed in the project directory:

    glew32.dll
    SDL2.dll
