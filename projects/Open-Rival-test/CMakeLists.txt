cmake_minimum_required (VERSION 3.16)

# Pull in auto-generated list of source/header files
set(OPEN_RIVAL_DIR ${CMAKE_SOURCE_DIR}/projects/Open-Rival)
include(${CMAKE_SOURCE_DIR}/CMake/OpenRivalFiles.cmake)

# Remove Open-Rival's Main.cpp so it doesn't conflict with our own main method
list(REMOVE_ITEM OPEN_RIVAL_HEADERS ${OPEN_RIVAL_DIR}/src/main/Main.cpp)

# Test framework
set(OPEN_RIVAL_TEST_FRAMEWORK_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/catch2/catch.h
)

# Replacement headers to override Open-Rival headers
set(OPEN_RIVAL_REPLACEMENT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/gfx/MockSDL.h
    ${CMAKE_CURRENT_LIST_DIR}/include/gl/glew.h
)

# Replacement sources to override Open-Rival sources
set(OPEN_RIVAL_REPLACEMENT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/application/Resources.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/application/Window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/audio/AudioSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/gfx/Font.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/gfx/MockSDL.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/gfx/Texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/gl/glew.cpp
)

# Tests and utilities
set(OPEN_RIVAL_TEST_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/main/Main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestAnimationComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestApplication.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestCamera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestEntity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestMapUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestMousePicker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestRenderUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestSpritesheet.cpp
)

set(OPEN_RIVAL_TEST_INCLUDE_DIRECTORIES
    ${OPEN_RIVAL_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/include
)

set(OPEN_RIVAL_TEST_LIBARY_INCLUDES
    # We include some library headers because we need the definitions, but we
    # don't actually link with the library. This works fine until we try to use
    # the library functionality, at which point we need to provide mock
    # implementations.
    ${LIBS_DIR}/freetype-${FREETYPE_VERSION}/include
    ${LIBS_DIR}/openal-soft-${OPENAL_VERSION}/include
    ${LIBS_DIR}/SDL2-${SDL2_VERSION}/include
    ${LIBS_DIR}/SDL2_image-${SDL2_IMAGE_VERSION}/include
    ${LIBS_DIR}/rtmidi-${RTMIDI_VERSION}
)

# Organise files into folders (filters)
source_group(TREE ${OPEN_RIVAL_DIR}/src PREFIX "Open-Rival Source Files" FILES ${OPEN_RIVAL_SOURCES})
source_group(TREE ${OPEN_RIVAL_DIR}/include PREFIX "Open-Rival Source Files" FILES ${OPEN_RIVAL_HEADERS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/include PREFIX "Test Framework" FILES ${OPEN_RIVAL_TEST_FRAMEWORK_HEADERS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "Replacement Source Files" FILES ${OPEN_RIVAL_REPLACEMENT_SOURCES})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/include PREFIX "Replacement Source Files" FILES ${OPEN_RIVAL_REPLACEMENT_HEADERS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "Test Source Files" FILES ${OPEN_RIVAL_TEST_SOURCES})

# Creates the executable including all the sources and resources
add_executable(Open-Rival-test
    ${OPEN_RIVAL_SOURCES}
    ${OPEN_RIVAL_HEADERS}
    ${OPEN_RIVAL_TEST_FRAMEWORK_HEADERS}
    ${OPEN_RIVAL_REPLACEMENT_SOURCES}
    ${OPEN_RIVAL_REPLACEMENT_HEADERS}
    ${OPEN_RIVAL_TEST_SOURCES}
)

# Add the directories and headers of the project
target_include_directories(Open-Rival-test PRIVATE
    ${OPEN_RIVAL_TEST_INCLUDE_DIRECTORIES}
    ${OPEN_RIVAL_TEST_LIBARY_INCLUDES}
)

# Dependencies (these ones are pretty lightweight so we just use them directly)
add_glm(Open-Rival-test)
add_json(Open-Rival-test)
add_spdlog(Open-Rival-test)

# Adds the libraries it must link against to find the symbols
target_link_libraries(Open-Rival-test PRIVATE
    Ws2_32
    project_options
    project_warnings
)

# Pre-compiled headers
target_precompile_headers(Open-Rival-test PRIVATE
    <cstdint>
    <memory>
    <stdexcept>
    <string>
    <vector>
)
