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

# DLLs folder
set(OPEN_RIVAL_DLL_SRC_DIR "${CMAKE_SOURCE_DIR}/dist")

# Resources folder
set(OPEN_RIVAL_RES_SRC_DIR "${CMAKE_SOURCE_DIR}/dist/res")

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

# Find dependencies
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(OpenAL REQUIRED)
find_package(rtmidi REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(spdlog REQUIRED)

# Link dependencies
target_link_libraries(Open-Rival PRIVATE
    Freetype::Freetype
    GLEW::glew
    glm::glm
	glu32
    nlohmann_json::nlohmann_json
    OpenAL::OpenAL
	opengl32
    RtMidi::rtmidi
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image::SDL2_image
    spdlog::spdlog
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

################################################################################
# Resources
# ----------
# These are copied to the build folder, and changes to these files will trigger
# a new CMake build.
################################################################################

# Find all resources
file(GLOB_RECURSE RESOURCE_LIST "${OPEN_RIVAL_RES_SRC_DIR}/*")

# Create a custom command to copy resources
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${OPEN_RIVAL_RES_SRC_DIR} ${CMAKE_CURRENT_BINARY_DIR}/res
    DEPENDS ${RESOURCE_LIST}
    COMMENT "Copying resources to build directory"
)

# Create a custom target that depends on the custom command
add_custom_target(copy_assets ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/res)

# Ensure the custom target runs before Open-Rival
add_dependencies(Open-Rival copy_assets)

################################################################################
# Extra files
# ------------
# These are copied to the build folder when CMake runs, but changes to these
# files will not trigger a new CMake build.
################################################################################

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
