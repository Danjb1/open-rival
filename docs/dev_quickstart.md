# Development Quickstart

This is a guide to getting started with Open-Rival development.

Please also read the [contributing guidelines](/docs/CONTRIBUTING.md).

## :world_map: Project Overview

Open-Rival contains a number of projects alongside the game itself, to assist with resource extraction, etc.

### [Open-Rival](/projects/Open-Rival)

> Rival Realms remake (_the game!_).

### [Open-Rival-test](/projects/Open-Rival-test)

> Unit tests for Open-Rival.

### [setup](/projects/setup)

> Installer for Open-Rival, that combines the various utilities listed below.

### [audio-extractor](/projects/audio-extractor)

> Standalone utility to extract the original game's audio.

### [campaign-extractor](/projects/campaign-extractor)

> Standalone utility to extract individual scenarios from campaign files.

### [image-extractor](/projects/image-extractor)

> Standalone utility to extract the original game's images.

### [interface-extractor](/projects/interface-extractor)

> Standalone utility to extract the original game's interface images.

### [texture-builder](/projects/texture-builder)

> Standalone utility to combine images into textures.

## :hammer: Build

These are the steps necessary in order to build and run Open-Rival.

### 1) Install vcpkg

[vcpkg](https://github.com/microsoft/vcpkg) is used to download the [required dependencies](/docs/dependencies.md) automatically.

Install it somewhere on your PC by following step 1 [here](https://learn.microsoft.com/en-gb/vcpkg/get_started/get-started?pivots=shell-powershell#1---set-up-vcpkg).

After installation, set the environment variable `VCPKG_ROOT` to point to the vcpkg root directory.

### 2) Install CMake

[CMake](https://cmake.org/) is used to build the project files necessary for compilation (for example, a Visual Studio solution).

### 3) Run CMake

Run CMake using this preset to generate a Visual Studio 2022 solution:

```
cmake --preset windows-vs2022-x64
```

### 4) Build Solution

At this point, it should be possible to build the solution using Visual Studio. However, do not attempt to run the game yet, as we still need to extract the required resources.

### 5) Extract Resources

To extract the required resources, build and run the [setup project](/projects/setup).

> This should "just work" as long as Rival Realms is installed, but there are further instuctions in the [readme](/projects/setup/README.md)) if needed!

This should create a `res` folder in the build directory, e.g. `build/windows-vs2022-x64/projects/setup/res`.

Copy this folder to the Open-Rival build directory (e.g. `build/windows-vs2022-x64/projects/Open-Rival/res`).

> Some resources are custom-made for, or bundled with, Open-Rival. These are included in the `dist/res` directory. When compiling the project, CMake will automatically copy the contents of this directory to the build folder for use during development.

### 6) That's it! :tada:

Once the dependencies and resources are in place, you should be able to run the "Open-Rival" project!

## :mag: Static Code Analysis

[Cppcheck](http://cppcheck.sourceforge.net/) should be run periodically.

> These scripts can also be found in the [tools](/tools) folder.

### Quick

```
cppcheck --project=Open-Rival.vcxproj "--project-configuration=Release|Win32"
```

### Full

```
cppcheck --project=Open-Rival.vcxproj
```

## :boom: Release

Instructions on building and packaging a version for release can be found [here](/docs/release_checklist.md).
