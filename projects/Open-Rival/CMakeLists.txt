cmake_minimum_required (VERSION 3.16)

# Pull in auto-generated list of source/header files
set(OPEN_RIVAL_DIR "${CMAKE_CURRENT_LIST_DIR}")
include("${CMAKE_SOURCE_DIR}/CMake/OpenRivalFiles.cmake")

# Docs (not required for compilation, but helpful to have at hand)
set(OPEN_RIVAL_DOCS
    "${CMAKE_SOURCE_DIR}/dist/docs/changelog.txt"
    "${CMAKE_SOURCE_DIR}/docs/TODO.md"
)

# Extra project files (not required for compilation, but helpful to have at hand)
set(OPEN_RIVAL_EXTRA_FILES
    "${CMAKE_SOURCE_DIR}/projects/.clang-format"
)

# Include directories
set(OPEN_RIVAL_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_LIST_DIR}/"
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

# C++ resource files
set(OPEN_RIVAL_RESOURCES
    "${CMAKE_CURRENT_LIST_DIR}/Open-Rival.rc"
    "${CMAKE_CURRENT_LIST_DIR}/resource.h"
)

# Resources folder
set(OPEN_RIVAL_RES_SRC_DIR "${CMAKE_SOURCE_DIR}/dist/res")

# DLLs folder
set(OPEN_RIVAL_DLL_SRC_DIR "${CMAKE_SOURCE_DIR}/dist")

# Config file
set(OPEN_RIVAL_CONFIG_SRC "${CMAKE_SOURCE_DIR}/dist/config.json")

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

# Libraries to link against
target_link_libraries(Open-Rival PRIVATE
    winmm
    Ws2_32
    project_options
    project_warnings
)

# Add macro definitions
target_compile_definitions(Open-Rival PRIVATE
    # Define DEBUG macro in debug builds
    $<$<CONFIG:Debug>:DEBUG>
)

# Pre-compiled headers
target_precompile_headers(Open-Rival PRIVATE
    <cstdint>
    <memory>
    <stdexcept>
    <string>
    <vector>
)

# Copy resources to build folder
file(COPY ${OPEN_RIVAL_RES_SRC_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Copy DLL files to build folder
file(GLOB OPEN_RIVAL_DLL_FILES "${OPEN_RIVAL_DLL_SRC_DIR}/*.dll")
foreach(DLL ${OPEN_RIVAL_DLL_FILES})
    file(COPY ${DLL} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

# Copy config file to build folder (if not present)
if(NOT(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/config.json))
    file(COPY ${OPEN_RIVAL_CONFIG_SRC} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

# Copy dev files to project folder
foreach(FILE ${OPEN_RIVAL_DEV_FILES})
    if(EXISTS ${FILE})
        file(COPY ${FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    endif()
endforeach()
