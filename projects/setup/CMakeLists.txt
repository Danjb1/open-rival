cmake_minimum_required (VERSION 3.16)

set(OPEN_RIVAL_SRC_DIR  ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/src)
set(OPEN_RIVAL_INC_DIR  ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include)
set(OPEN_RIVAL_JSON_DIR ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/libs/json)

set(OPEN_RIVAL_SETUP_EXTERNAL_SOURCES
    ${OPEN_RIVAL_SRC_DIR}/BinaryFileReader.cpp
    ${OPEN_RIVAL_SRC_DIR}/FileUtils.cpp
    ${OPEN_RIVAL_SRC_DIR}/Image.cpp
    ${OPEN_RIVAL_SRC_DIR}/MathUtils.cpp
    ${OPEN_RIVAL_SRC_DIR}/Palette.cpp
    ${OPEN_RIVAL_SRC_DIR}/StringUtils.cpp
)

set(OPEN_RIVAL_SETUP_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/audio-extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/image-extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/interface-extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/texture-builder.cpp
    # Shall this be here?
    ${CMAKE_CURRENT_LIST_DIR}/src/pch.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/registry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/setup-utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Main.cpp
)

set(OPEN_RIVAL_SETUP_EXTERNAL_INCLUDE_HEADERS
    ${OPEN_RIVAL_INC_DIR}/BinaryFileReader.h
    ${OPEN_RIVAL_INC_DIR}/FileUtils.h
    ${OPEN_RIVAL_INC_DIR}/Image.h
    ${OPEN_RIVAL_INC_DIR}/MathUtils.h
    ${OPEN_RIVAL_INC_DIR}/Palette.h
    ${OPEN_RIVAL_INC_DIR}/StringUtils.h
)

set(OPEN_RIVAL_SETUP_INCLUDE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/audio-extractor.h
    ${CMAKE_CURRENT_LIST_DIR}/include/image-extractor.h
    ${CMAKE_CURRENT_LIST_DIR}/include/interface-extractor.h
    ${CMAKE_CURRENT_LIST_DIR}/include/registry.h
    ${CMAKE_CURRENT_LIST_DIR}/include/setup-utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/texture-builder.h
)

set(OPEN_RIVAL_SETUP_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/include
)

set(OPEN_RIVAL_SETUP_PRECOMPILED_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/pch.h
)

# Creates the executable
add_executable(setup
    ${OPEN_RIVAL_SETUP_EXTERNAL_SOURCES}
    ${OPEN_RIVAL_SETUP_SOURCES}
    ${OPEN_RIVAL_SETUP_INCLUDE_HEADERS}
    ${OPEN_RIVAL_SETUP_EXTERNAL_INCLUDE_HEADERS}
)

target_include_directories(setup PUBLIC
    ${OPEN_RIVAL_SETUP_INCLUDE_DIRS}
)

target_include_directories(setup PRIVATE
    ${OPEN_RIVAL_INC_DIR}
    ${OPEN_RIVAL_JSON_DIR}
)

target_link_libraries(setup PRIVATE
    project_options
    project_warnings
)

target_precompile_headers(setup PRIVATE
    ${OPEN_RIVAL_SETUP_PRECOMPILED_HEADERS}
)
