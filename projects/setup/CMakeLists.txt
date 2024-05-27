cmake_minimum_required (VERSION 3.16)

set(OPEN_RIVAL_SRC_DIR  ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/src)
set(OPEN_RIVAL_INC_DIR  ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include)

set(SETUP_EXTERNAL_SOURCES
    ${OPEN_RIVAL_SRC_DIR}/gfx/Image.cpp
    ${OPEN_RIVAL_SRC_DIR}/gfx/Palette.cpp
    ${OPEN_RIVAL_SRC_DIR}/utils/BinaryFileReader.cpp
    ${OPEN_RIVAL_SRC_DIR}/utils/FileUtils.cpp
    ${OPEN_RIVAL_SRC_DIR}/utils/LogUtils.cpp
    ${OPEN_RIVAL_SRC_DIR}/utils/MathUtils.cpp
    ${OPEN_RIVAL_SRC_DIR}/utils/StringUtils.cpp
)

set(SETUP_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/audio-extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/image-extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ImageGen.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/interface-extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/texture-builder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/registry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/setup-utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Main.cpp
)

set(SETUP_EXTERNAL_INCLUDE_HEADERS
    ${OPEN_RIVAL_INC_DIR}/gfx/Image.h
    ${OPEN_RIVAL_INC_DIR}/gfx/Palette.h
    ${OPEN_RIVAL_INC_DIR}/utils/BinaryFileReader.h
    ${OPEN_RIVAL_INC_DIR}/utils/FileUtils.h
    ${OPEN_RIVAL_INC_DIR}/utils/LogUtils.h
    ${OPEN_RIVAL_INC_DIR}/utils/MathUtils.h
    ${OPEN_RIVAL_INC_DIR}/utils/StringUtils.h
)

set(SETUP_INCLUDE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/audio-extractor.h
    ${CMAKE_CURRENT_LIST_DIR}/include/image-extractor.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ImageGen.h
    ${CMAKE_CURRENT_LIST_DIR}/include/interface-extractor.h
    ${CMAKE_CURRENT_LIST_DIR}/include/registry.h
    ${CMAKE_CURRENT_LIST_DIR}/include/setup-utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/texture-builder.h
)

set(SETUP_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# "setup" folder
set(SETUP_EXTRAS_DIR "${CMAKE_SOURCE_DIR}/dist/setup")

# Creates the executable
add_executable(setup
    ${SETUP_EXTERNAL_SOURCES}
    ${SETUP_SOURCES}
    ${SETUP_INCLUDE_HEADERS}
    ${SETUP_EXTERNAL_INCLUDE_HEADERS}
)

target_include_directories(setup PUBLIC
    ${SETUP_INCLUDE_DIRS}
)

target_include_directories(setup PRIVATE
    ${OPEN_RIVAL_INC_DIR}
)

# Dependencies
add_spdlog(setup)

target_link_libraries(setup PRIVATE
    project_options
    project_warnings
)

# Use Unicode encoding on Windows
if (MSVC)
    target_compile_definitions(setup PUBLIC UNICODE _UNICODE)
endif()

# Copy "setup" folder to build folder
file(COPY ${SETUP_EXTRAS_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
