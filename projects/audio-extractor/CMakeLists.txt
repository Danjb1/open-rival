cmake_minimum_required (VERSION 3.16)

set(OPEN_RIVAL_SETUP_SRC_DIR  ${CMAKE_CURRENT_LIST_DIR}/../setup/src)
set(OPEN_RIVAL_SETUP_INC_DIR  ${CMAKE_CURRENT_LIST_DIR}/../setup/include)

set(OPEN_RIVAL_AUDIO_EXTRACTOR_EXTERNAL_SOURCES
    ${OPEN_RIVAL_SETUP_SRC_DIR}/audio-extractor.cpp
)

set(OPEN_RIVAL_AUDIO_EXTRACTOR_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
)

set(OPEN_RIVAL_AUDIO_EXTRACTOR_EXTERNAL_INCLUDE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/../setup/include/audio-extractor.h
)

set(OPEN_RIVAL_AUDIO_EXTRACTOR_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}
)

# Creates the target
add_executable(audio-extractor
    ${OPEN_RIVAL_AUDIO_EXTRACTOR_EXTERNAL_SOURCES}
    ${OPEN_RIVAL_AUDIO_EXTRACTOR_SOURCES}
    ${OPEN_RIVAL_AUDIO_EXTRACTOR_EXTERNAL_INCLUDE_HEADERS}
)

target_include_directories(audio-extractor PUBLIC
    ${OPEN_RIVAL_AUDIO_EXTRACTOR_INCLUDE_DIRS}
    ${OPEN_RIVAL_SETUP_INC_DIR}
)

target_link_libraries(audio-extractor PRIVATE
    project_options
    project_warnings
)
