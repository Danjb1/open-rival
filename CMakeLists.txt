cmake_minimum_required(VERSION 3.10)
project(Open-Rival CXX)

include(CMake/CompilerWarnings.cmake)
include(CMake/StandardProjectSettings.cmake)

set(CMAKE_CXX_STANDARD 20)

MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

# Library directory
set(LIBS_DIR "${CMAKE_SOURCE_DIR}/libs" CACHE STRING "Library directory")

# This is seemingly an empty interface which all projects depend on... remove?
add_library(project_options INTERFACE)

# Option specifications
option(ENABLE_TESTING "Enable Test Builds" OFF)

# Projects can link with this "library" to use the warnings
# specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

# Add projects
add_subdirectory(projects/Open-Rival)
add_subdirectory(projects/Open-Rival-test)
add_subdirectory(projects/setup)
add_subdirectory(projects/audio-extractor)
add_subdirectory(projects/campaign-extractor)
add_subdirectory(projects/image-extractor)
add_subdirectory(projects/interface-extractor)
add_subdirectory(projects/texture-builder)

# Set start-up project for Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Open-Rival)
