cmake_minimum_required(VERSION 3.25)
project(Open-Rival
    VERSION 0.5.0
    DESCRIPTION "Faithful, modernized reimplementation of the 1998 RTS game Rival Realms"
    LANGUAGES CXX
)

include(CMake/CompilerWarnings.cmake)
include(CMake/StandardProjectSettings.cmake)

MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

################################################################################
# Options
################################################################################

option(BUILD_TESTING "Build the unit tests" ON)
option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)

################################################################################
# Common project settings
################################################################################

# Projects can link with this "library" to use our common compiler settings
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compile source files in parallel to speed up builds
target_compile_options(project_options INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/MP>)

# Embed debug information in object files (/Z7), instead of writing it to a separate PDB during compilation (/Zi).
# This is required for the test project to reuse the library's pre-compiled headers: with /Zi, CMake copies the
# library's compile-time PDB into the test project before each build, which corrupts the test project's own debug
# information. The linker still produces a PDB for each executable, so debugging is unaffected.
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>")

# Projects can link with this "library" to use the warnings
# specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

################################################################################
# Projects
################################################################################

add_subdirectory(projects/Open-Rival)
if(BUILD_TESTING)
    # Registers tests with CTest (e.g. for `ctest` or the Visual Studio Test Explorer)
    enable_testing()
    add_subdirectory(projects/Open-Rival-test)
endif()
add_subdirectory(projects/setup)
add_subdirectory(projects/audio-extractor)
add_subdirectory(projects/campaign-extractor)
add_subdirectory(projects/image-extractor)
add_subdirectory(projects/interface-extractor)
add_subdirectory(projects/texture-builder)

# Set start-up project for Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Open-Rival)
