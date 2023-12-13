cmake_minimum_required (VERSION 3.16)

set(OPEN_RIVAL_SRC_DIR          ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/src)
set(OPEN_RIVAL_SETUP_SRC_DIR    ${CMAKE_CURRENT_LIST_DIR}/../setup/src)

set(OPEN_RIVAL_TEXTURE_BUILDER_EXTERNAL_SOURCES
    ${OPEN_RIVAL_SRC_DIR}/gfx/Image.cpp
    ${OPEN_RIVAL_SRC_DIR}/gfx/Palette.cpp
    ${OPEN_RIVAL_SRC_DIR}/utils/BinaryFileReader.cpp
    ${OPEN_RIVAL_SRC_DIR}/utils/MathUtils.cpp
    ${OPEN_RIVAL_SETUP_SRC_DIR}/setup-utils.cpp
    ${OPEN_RIVAL_SETUP_SRC_DIR}/texture-builder.cpp
)

set(OPEN_RIVAL_TEXTURE_BUILDER_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
)

set(OPEN_RIVAL_TEXTURE_BUILDER_EXTERNAL_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include
    ${CMAKE_CURRENT_LIST_DIR}/../setup/include
)

set(OPEN_RIVAL_TEXTURE_BUILDER_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}
)

set(OPEN_RIVAL_TEXTURE_BUILDER_EXTERNAL_INCLUDE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/gfx/Image.h
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/gfx/Palette.h
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/utils/BinaryFileReader.h
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/utils/MathUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/../setup/include/setup-utils.h
    ${CMAKE_CURRENT_LIST_DIR}/../setup/include/texture-builder.h
)

# Creates the executable
add_executable(texture-builder
    ${OPEN_RIVAL_TEXTURE_BUILDER_SOURCES}
    ${OPEN_RIVAL_TEXTURE_BUILDER_EXTERNAL_SOURCES}
    ${OPEN_RIVAL_TEXTURE_BUILDER_EXTERNAL_INCLUDE_HEADERS}
)
target_include_directories(texture-builder PUBLIC
    ${OPEN_RIVAL_TEXTURE_BUILDER_INCLUDE_DIRS}
    ${OPEN_RIVAL_TEXTURE_BUILDER_EXTERNAL_DIRS}
)

# Dependencies
add_spdlog(texture-builder)

target_link_libraries(texture-builder PRIVATE
    project_options
    project_warnings
)
