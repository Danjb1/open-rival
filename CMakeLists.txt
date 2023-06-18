cmake_minimum_required(VERSION 3.16)
project(open-rival CXX)
include(CMake/StandardProjectSettings.cmake)

set(CMAKE_CXX_STANDARD 17)

add_library(project_options INTERFACE)

# Option specifications
option(ENABLE_TESTING "Enable Test Builds" OFF)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(CMake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

#include sub-projects
add_subdirectory(projects/Open-Rival)
add_subdirectory(projects/setup)
add_subdirectory(projects/audio-extractor)
add_subdirectory(projects/campaign-extractor)
add_subdirectory(projects/image-extractor)
add_subdirectory(projects/interface-extractor)
add_subdirectory(projects/texture-builder)
