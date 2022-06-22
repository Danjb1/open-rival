# Open-Rival-test

Unit tests for Open-Rival using the [Catch2](https://github.com/catchorg/Catch2) test framework (included).

## Build & Run

The project can be built and run using Visual Studio.

This should display a message with the outcome of the tests.

## Project Setup

The test project includes all headers required by Open-Rival, but does *not* include all of the source files. In particular, most of our third-party libraries are omitted. This is to help keep the tests lightweight; we do not want to create an OpenGL context every time we run our tests.

This means that we have to provide stub implementations for any missing definitions that we depend upon. For the most part, source files from Open-Rival can be directly included in the test project, as required.

To keep the project organised, several filters have been created:

 - **Source Files:** Unmodified source files under test, imported directly from Open-Rival.
 - **Test Doubles:** Test-only implementations of Open-Rival headers.
 - **Test Framework:** Files required to get the tests to run.
 - **Tests:** The tests themselves.

## Troubleshooting

Sometimes the tests fail to run because Windows Defender blocks them for some reason.

Restarting Visual Studio seems to fix the issue.
