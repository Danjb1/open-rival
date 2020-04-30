# Tasks

 - Perform static analysis
 - Run unit tests
 - Write a build script that includes these steps
 - Update the README accordingly

## Static Analysis

Use cppcheck:

http://cppcheck.sourceforge.net/
http://cppcheck.sourceforge.net/manual.pdf
https://github.com/VioletGiraffe/cppcheck-vs-addin

This can be used in Visual Studio directly, or through the command-line:

    cppcheck --project=MyProj.vcxproj

## Unit Tests

Visual Studio includes some testing frameworks out-of-the-box.

https://docs.microsoft.com/en-us/visualstudio/test/writing-unit-tests-for-c-cpp?view=vs-2019

Much like MSBuild, there is a command-line utility, MSTest.exe, to run unit tests.

https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/ms182489(v=vs.100)

    MSTest.exe  /testcontainer:BankAccountTest.dll

## Build Script

MSBuild is used by Visual Studio, but can be used to build a Visual Studio project from the command-line:

    MSBuild.exe MyProj.proj -property:Configuration=Debug

https://docs.microsoft.com/en-gb/visualstudio/msbuild/msbuild?view=vs-2019

A simple script can be written to run the static analysis and build the project.

    #!/usr/bin/env bash

    # Run static analysis
    lintWarnings=$(cppcheck --project=MyProj.vcxproj)
    if [ -z "$lintWarnings" ]; then
        echo "Failed"
    fi

    # Compile
    MSBuild.exe MyProj.proj -property:Configuration=Debug
