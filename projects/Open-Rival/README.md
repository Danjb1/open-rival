# Open Rival

## Build

Configure the project using CMake and build using any modern compiler.

See [this guide](/docs/dev_quickstart.md) for more details.

## Static Code Analysis

### [Cppcheck](http://cppcheck.sourceforge.net/)

This should be run periodically.

> These scripts can also be found in the [tools](/tools) folder.

#### Quick

    cppcheck --project=Open-Rival.vcxproj "--project-configuration=Release|Win32"

#### Full

    cppcheck --project=Open-Rival.vcxproj
