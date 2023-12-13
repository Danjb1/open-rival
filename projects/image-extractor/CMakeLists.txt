cmake_minimum_required (VERSION 3.16)

set(OPEN_RIVAL_SRC_DIR          ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/src)
set(OPEN_RIVAL_SETUP_SRC_DIR    ${CMAKE_CURRENT_LIST_DIR}/../setup/src)

set(OPEN_RIVAL_IMAGE_EXTRACTOR_EXTERNAL_SOURCES
    ${OPEN_RIVAL_SRC_DIR}/gfx/Image.cpp
    ${OPEN_RIVAL_SRC_DIR}/gfx/Palette.cpp
    ${OPEN_RIVAL_SRC_DIR}/utils/BinaryFileReader.cpp
    ${OPEN_RIVAL_SRC_DIR}/utils/FileUtils.cpp
    ${OPEN_RIVAL_SRC_DIR}/utils/MathUtils.cpp
    ${OPEN_RIVAL_SETUP_SRC_DIR}/image-extractor.cpp
    ${OPEN_RIVAL_SETUP_SRC_DIR}/setup-utils.cpp
)

set(OPEN_RIVAL_IMAGE_EXTRACTOR_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
)

set(OPEN_RIVAL_IMAGE_EXTRACTOR_EXTERNAL_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/../setup/include
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include
)

set(OPEN_RIVAL_IMAGE_EXTRACTOR_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}
)

set(OPEN_RIVAL_IMAGE_EXTRACTOR_EXTERNAL_INCLUDE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/gfx/Image.h
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/gfx/Palette.h
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/utils/BinaryFileReader.h
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/utils/FileUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/utils/MathUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/../setup/include/image-extractor.h
    ${CMAKE_CURRENT_LIST_DIR}/../setup/include/setup-utils.h
)

# Creates the target
add_executable(image-extractor
    ${OPEN_RIVAL_IMAGE_EXTRACTOR_EXTERNAL_SOURCES}
    ${OPEN_RIVAL_IMAGE_EXTRACTOR_SOURCES}
    ${OPEN_RIVAL_IMAGE_EXTRACTOR_EXTERNAL_INCLUDE_HEADERS}
)

target_include_directories(image-extractor PUBLIC
    ${OPEN_RIVAL_IMAGE_EXTRACTOR_INCLUDE_DIRS}
    ${OPEN_RIVAL_IMAGE_EXTRACTOR_EXTERNAL_DIRS}
)

# Dependencies
add_spdlog(image-extractor)

target_link_libraries(image-extractor PRIVATE
    project_options
    project_warnings
)
