# Open Rival

## Dependencies

 - [glew](http://glew.sourceforge.net/)

 - [SDL2](https://www.libsdl.org/download-2.0.php)

 - [SDL_image](https://www.libsdl.org/projects/SDL_image/)

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
