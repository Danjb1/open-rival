# Open Rival

## Build

Build using Visual Studio after installing all required dependencies.

See [this guide](/docs/development_quickstart.md) for more details.

## Static Code Analysis

### [Cppcheck](http://cppcheck.sourceforge.net/)

This should be run periodically.

> These scripts can also be found in the [tools](/tools) folder.

#### Quick

    cppcheck --project=Open-Rival.vcxproj "--project-configuration=Release|Win32"

#### Full

    cppcheck --project=Open-Rival.vcxproj
