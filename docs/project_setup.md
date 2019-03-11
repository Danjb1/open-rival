# Project Setup

This is a guide to configuring the project with the required dependencies, for **x86 platforms**.

## Dependencies

These tutorials describe how to install each of our dependencies:

 - [glew](http://lazyfoo.net/tutorials/SDL/51_SDL_and_modern_opengl/index.php)

 - [SDL2](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php)

 - [SDL_image](http://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php)

## Directory Structure

These dependencies should be placed in the "libs" folder:

    +-- Open-Rival/
        +-- libs/
            +-- glew-2.1.0/
            +-- SDL2_image-2.0.3/
            +-- SDL2-2.0.8/

## Include Directories

    libs/SDL2-2.0.8/include
    libs/SDL2_image-2.0.3/include
    libs/glew-2.1.0/include

## Library Directories

    libs/SDL2-2.0.8/lib/x86
    libs/SDL2_image-2.0.3/lib/x86
    libs/glew-2.1.0/lib/Release/Win32

## Linker Additional Dependencies

    glew32.lib
    glu32.lib
    opengl32.lib
    SDL2.lib
    SDL2main.lib
    SDL2_image.lib

## DLL Files

These should be placed in the project directory:

    glew32.dll
    libjpeg-9.dll
    libpng16-16.dll
    libtiff-5.dll
    libwebp-7.dll
    SDL2.dll
    SDL2_image.dll
    zlib1.dll
