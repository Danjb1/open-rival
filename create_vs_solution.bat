@echo off

REM Set the build type based on the command-line argument
set "BUILD_TYPE=%~1"
if "%BUILD_TYPE%"=="" set "BUILD_TYPE=debug"
if /I "%BUILD_TYPE%" neq "debug" if /I "%BUILD_TYPE%" neq "release" set "BUILD_TYPE=debug"

set "PLATFORM=%~2"
if /I "%PLATFORM%" neq "x64" if /I "%PLATFORM%" neq "Win32" set "PLATFORM=Win32"

set "SOURCE_DIR=%~dp0"
set "BUILD_DIR=%SOURCE_DIR%\cmake-build-%BUILD_TYPE%-%PLATFORM%"

REM Create the build directory
mkdir "%BUILD_DIR%"

REM Change to the build directory
cd /D "%BUILD_DIR%"

REM Configure the CMake project
cmake -A "%PLATFORM%" ..
cd ..
