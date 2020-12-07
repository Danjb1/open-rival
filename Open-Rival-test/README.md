# Open-Rival-test

Unit tests for Open-Rival using the [Catch2](https://github.com/catchorg/Catch2) test framework (included).

## Build & Run

The project can be built and run using Visual Studio.

This should display a message with the outcome of the tests.

## Project Setup

The test project includes all headers defined by Open-Rival, but does *not* include the headers from most of our third-party libraries. This is to help keep the tests lightweight; we do not want to create an OpenGL context every time we run our tests.

This means that we have to provide stub implementations for these missing third-party definitions that we depend upon. Stub or mock implementations of Open-Rival definitions can also be provided for files that depend heavily on third-party libraries (e.g. `Texture`). Other source files from Open-Rival can be directly included in the test project, as required.

To keep the project organised, several filters have been created:

 - **Test Framework:** Files required to get the tests to run.
 - **Source Files:** Unmodified source files under test, imported directly from Open-Rival.
 - **Test Doubles:** Test-only implementations of Open-Rival headers.
 - **Tests:** The tests themselves.

## Troubleshooting

Sometimes the tests fail to run because Windows Defender blocks them for some reason.

Restarting Visual Studio seems to fix the issue.
