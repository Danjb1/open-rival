cmake_minimum_required (VERSION 3.16)

# Pull in auto-generated list of source/header files
set(OPEN_RIVAL_DIR ${CMAKE_CURRENT_LIST_DIR})
include(${CMAKE_SOURCE_DIR}/CMake/OpenRivalFiles.cmake)

# Not required for compilation, but helpful to have at hand
set(OPEN_RIVAL_DOCS
    ${CMAKE_SOURCE_DIR}/dist/docs/changelog.txt
    ${CMAKE_SOURCE_DIR}/docs/TODO.md
)

# Not required for compilation, but helpful to have at hand
set(OPEN_RIVAL_EXTRA_FILES
    ${CMAKE_SOURCE_DIR}/projects/.clang-format
)

set(OPEN_RIVAL_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_LIST_DIR}/
    ${CMAKE_CURRENT_LIST_DIR}/include
)

set(OPEN_RIVAL_RESOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Open-Rival.rc
    ${CMAKE_CURRENT_LIST_DIR}/resource.h
)

# Organise files into folders (filters)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "Source Files" FILES ${OPEN_RIVAL_SOURCES})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/include PREFIX "Source Files" FILES ${OPEN_RIVAL_HEADERS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} PREFIX "Resources" FILES ${OPEN_RIVAL_RESOURCES})
source_group("Docs" FILES ${OPEN_RIVAL_DOCS})

# Creates the executable including all the sources and resources
add_executable(Open-Rival
    ${OPEN_RIVAL_SOURCES}
    ${OPEN_RIVAL_HEADERS}
    ${OPEN_RIVAL_RESOURCES}
    ${OPEN_RIVAL_DOCS}
    ${OPEN_RIVAL_EXTRA_FILES}
)

# Add the directories and headers of the project
target_include_directories(Open-Rival PRIVATE
    ${OPEN_RIVAL_INCLUDE_DIRECTORIES}
)

# Dependencies
add_freetype(Open-Rival)
add_glew(Open-Rival)
add_glm(Open-Rival)
add_json(Open-Rival)
add_openal(Open-Rival)
add_rtmidi(Open-Rival)
add_sdl2(Open-Rival)
add_sdl2_image(Open-Rival)
add_spdlog(Open-Rival)

# Adds the libraries it must link against to find the symbols
target_link_libraries(Open-Rival PRIVATE
    Ws2_32
    project_options
    project_warnings
)

# Pre-compiled headers
target_precompile_headers(Open-Rival PRIVATE
    <cstdint>
    <memory>
    <stdexcept>
    <string>
    <vector>
)
