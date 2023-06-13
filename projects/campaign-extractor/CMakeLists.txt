cmake_minimum_required (VERSION 3.16)

set(OPEN_RIVAL_CAMPAIGN_EXTRACTOR_EXTERNAL_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/src/FileUtils.cpp
)

set(OPEN_RIVAL_CAMPAIGN_EXTRACTOR_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/campaign-extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pch.cpp
)

set(OPEN_RIVAL_CAMPAIGN_EXTRACTOR_EXTERNAL_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/libs/json
)

set(OPEN_RIVAL_CAMPAIGN_EXTRACTOR_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}
)

set(OPEN_RIVAL_CAMPAIGN_EXTRACTOR_PRECOMPILED_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/pch.h
)

# Creates the executable
add_executable(campaign-extractor
    ${OPEN_RIVAL_CAMPAIGN_EXTRACTOR_EXTERNAL_SOURCES}
    ${OPEN_RIVAL_CAMPAIGN_EXTRACTOR_SOURCES}
)
target_include_directories(campaign-extractor PUBLIC
    ${OPEN_RIVAL_CAMPAIGN_EXTRACTOR_INCLUDE_DIRS}
    ${OPEN_RIVAL_CAMPAIGN_EXTRACTOR_EXTERNAL_DIRS}
)


target_link_libraries(campaign-extractor PRIVATE
    project_options
    project_warnings
)

target_precompile_headers(campaign-extractor PRIVATE
    ${OPEN_RIVAL_CAMPAIGN_EXTRACTOR_PRECOMPILED_HEADERS}
)
