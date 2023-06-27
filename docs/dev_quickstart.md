# Development Quickstart

These are the steps necessary in order to build and run the project.

Instructions on building and packaging a version for release can be found [here](/docs/release_checklist.md).

## 1) Install & Run [CMake](https://cmake.org/)

The project is configured using CMake.

This should be run to produce the files necessary for compilation (for example, a Visual Studio solution).

## 2) Install Dependencies

Open-Rival has various dependencies, described [here](/docs/dependencies.md).

Generally, dependencies should be placed in `projects/Open-Rival/libs` (see example below).

The expected version numbers are listed in the [CMake build file](projects/Open-Rival/CMakeLists.txt).

For dynamically-linked libraries:
- Debug DLLs should be placed in the output directory (e.g. `build\projects\Open-Rival\Debug`).
- Release DLLs should be placed in the `dist` folder.

## 3) Install Resources

At this point, it should be possible to build the solution using Visual Studio. However, the game cannot run without the required resources. Most of these are extracted from the original game by the "setup" project.

### Setup Project

Build and run the "setup" project to perform resource extraction (instructions in the [readme](/setup/README.md)). This should create the folders `sound`, `textures` and `video` within the `setup/res` directory.

Copy these folders to the `projects/Open-Rival/res` directory.

### Custom Resources

Some resources are custom-made for Open-Rival. These are included in the `dist/res` directory. The contents of this directory should be copied to `projects/Open-Rival/res` for use during development.

### Font

The game uses the [Procopius font](https://fontsgeek.com/fonts/Procopius-Regular) which must be downloaded and installed separately.

## 4) That's it! :tada:

Once the dependencies and resources are in place, you should be able to run the "Open-Rival" project!
