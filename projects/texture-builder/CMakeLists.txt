cmake_minimum_required (VERSION 3.16)

set(OPEN_RIVAL_SRC_DIR          ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/src)
set(OPEN_RIVAL_SETUP_SRC_DIR    ${CMAKE_CURRENT_LIST_DIR}/../setup/src)

set(OPEN_RIVAL_TEXTURE_BUILDER_EXTERNAL_SOURCES
    ${OPEN_RIVAL_SRC_DIR}/BinaryFileReader.cpp
    ${OPEN_RIVAL_SRC_DIR}/Image.cpp
    ${OPEN_RIVAL_SRC_DIR}/MathUtils.cpp
    ${OPEN_RIVAL_SRC_DIR}/Palette.cpp
    ${OPEN_RIVAL_SETUP_SRC_DIR}/setup-utils.cpp
    ${OPEN_RIVAL_SETUP_SRC_DIR}/texture-builder.cpp
)

set(OPEN_RIVAL_TEXTURE_BUILDER_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pch.cpp
)

set(OPEN_RIVAL_TEXTURE_BUILDER_EXTERNAL_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include
    ${CMAKE_CURRENT_LIST_DIR}/../setup/include
)

set(OPEN_RIVAL_TEXTURE_BUILDER_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}
)

set(OPEN_RIVAL_TEXTURE_BUILDER_PRECOMPILED_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/pch.h
)

# Creates the executable
add_executable(texture-builder
    ${OPEN_RIVAL_TEXTURE_BUILDER_SOURCES}
    ${OPEN_RIVAL_TEXTURE_BUILDER_EXTERNAL_SOURCES}
)
target_include_directories(texture-builder PUBLIC
    ${OPEN_RIVAL_TEXTURE_BUILDER_INCLUDE_DIRS}
    ${OPEN_RIVAL_TEXTURE_BUILDER_EXTERNAL_DIRS}
)

target_link_libraries(texture-builder PRIVATE
    project_options
    project_warnings
)

target_precompile_headers(texture-builder PRIVATE
    ${OPEN_RIVAL_TEXTURE_BUILDER_PRECOMPILED_HEADERS}
)
