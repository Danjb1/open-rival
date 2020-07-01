# Contributing

Thank you for your interest in this project!

Contributions are very much appreciated. If you are not a programmer, you can still join our [Discord Server](https://discord.gg/R7E4KWT), give support and feedback, and help with testing.

## Getting Started

1. Fork the repository and download a local copy.

1. Install dependencies and build using the [Development Quickstart Guide](/docs/development.md).

1. Commit your changes (following the Developer Guidelines, below).

1. Create a pull request from your fork to the main repository.

## Developer Guidelines

Development of Open-Rival is driven by certain values which should be generally followed by all contributors.

### Stay Faithful

Apart from the identified ["Future Enhancements"](/docs/TODO.md), all features should aim to mimic the original game as closely as possible.

### Simplicity Over Optimisation

Unless performance is identified as a problem, prefer simplicity and readability to complex, but more optimal, solutions.

### Be Consistent

To ensure consistency within the codebase, code should be formatted using the included [`.clang-format`](/.clang-format) file.

> `clang-format.exe` can be downloaded as part of [LLVM](https://releases.llvm.org/download.html). Visual Studio should be configured to use this executable for its formatting, since the ones that ships with the IDE is outdated. There is also a [Format on Save]( https://marketplace.visualstudio.com/items?itemName=mynkow.FormatdocumentonSave) extension available.
