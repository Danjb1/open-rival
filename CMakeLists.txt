cmake_minimum_required(VERSION 3.16)
project(Open-Rival CXX)

include(CMake/AddFreetype.cmake)
include(CMake/AddGLEW.cmake)
include(CMake/AddGLM.cmake)
include(CMake/AddJson.cmake)
include(CMake/AddOpenAL.cmake)
include(CMake/AddRtMidi.cmake)
include(CMake/AddSDL2.cmake)
include(CMake/AddSDL2_Image.cmake)
include(CMake/AddSpdlog.cmake)
include(CMake/CompilerWarnings.cmake)
include(CMake/StandardProjectSettings.cmake)

set(CMAKE_CXX_STANDARD 20)

# Library directory
set(LIBS_DIR "${CMAKE_SOURCE_DIR}/libs" CACHE STRING "Library directory")

# Dependency versions
set(FREETYPE_VERSION "2.13.1" CACHE STRING "FreeType version number")
set(GLEW_VERSION "2.1.0" CACHE STRING "glew version number")
set(GLM_VERSION "0.9.9.8" CACHE STRING "GLM version number")
set(JSON_VERSION "3.11.2" CACHE STRING "JSON version number")
set(OPENAL_VERSION "1.23.1" CACHE STRING "OpenAL version number")
set(RTMIDI_VERSION "5.0.0" CACHE STRING "RtMidi version number")
set(SDL2_VERSION "2.28.0" CACHE STRING "SDL2 version number")
set(SDL2_IMAGE_VERSION "2.6.3" CACHE STRING "SDL2_Image version number")
set(SPDLOG_VERSION "1.12.0" CACHE STRING "spdlog version number")

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
set(VS_STARTUP_PROJECT, Open-Rival)
