# Open-Rival Tests

Unit tests for Open-Rival using the [Catch2](https://github.com/catchorg/Catch2) test framework (included).

## Project Setup

To keep the tests as lightweight as possible, we do not link with many of the libraries required by Open-Rival, and no window creation or rendering is performed. Instead, we provide mock definitions of any missing symbols to ensure that compilation can succeed. In some cases, we replace classes from Open-Rival with mock versions so that fewer symbols from these missing libraries are required.

## Troubleshooting

Sometimes the tests fail to run because Windows Defender blocks them for some reason.

Restarting Visual Studio seems to fix the issue.
