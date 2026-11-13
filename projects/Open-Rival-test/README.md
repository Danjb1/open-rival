# Open-Rival Tests

Unit tests for Open-Rival using the [Catch2](https://github.com/catchorg/Catch2) test framework (installed via vcpkg).

## Project Setup

All of the game code (everything except `main`) is built into the `Open-Rival-lib` static library, which is linked by both the game and the tests. This means the tests always run against the real game code.

The tests never create a window, OpenGL context or audio device. Where the game code depends on one of these systems, it does so through an interface (e.g. `Window`, `Renderer`, `AudioSystem`), and the tests supply their own implementations (see `include/mocks`).

## Running the Tests

Each test case is registered with CTest, so the tests can be run:

- From the Visual Studio Test Explorer.
- From the command line, using `ctest -C Debug` in the build folder.
- By running `Open-Rival-test.exe` directly (pass `-?` to see the available options).

The test project can be excluded from the build by setting the CMake option `BUILD_TESTING=OFF`.

## Troubleshooting

Sometimes the tests fail to run because Windows Defender blocks them for some reason.

Restarting Visual Studio seems to fix the issue.
