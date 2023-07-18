# Open-Rival Tests

Unit tests for Open-Rival using the [Catch2](https://github.com/catchorg/Catch2) test framework (included).

## Project Setup

The test project includes all Open-Rival headers and source files, but does *not* include any graphical libraries. This is to help keep the tests lightweight; we do not want to create an OpenGL context every time we run our tests. This means that we have to provide stub implementations for any missing definitions that we depend upon.

## Troubleshooting

Sometimes the tests fail to run because Windows Defender blocks them for some reason.

Restarting Visual Studio seems to fix the issue.
