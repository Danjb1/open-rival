cmake_minimum_required (VERSION 3.16)

set(OPEN_RIVAL_CAMPAIGN_EXTRACTOR_EXTERNAL_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/src/utils/FileUtils.cpp
)

set(OPEN_RIVAL_CAMPAIGN_EXTRACTOR_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/campaign-extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
)

set(OPEN_RIVAL_CAMPAIGN_EXTRACTOR_EXTERNAL_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include
)

set(OPEN_RIVAL_CAMPAIGN_EXTRACTOR_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}
)

set(OPEN_RIVAL_CAMPAIGN_EXTRACTOR_INCLUDE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/campaign-extractor.h
)

set(OPEN_RIVAL_CAMPAIGN_EXTRACTOR_EXTERNAL_INCLUDE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/../Open-Rival/include/utils/FileUtils.h
)

# Creates the executable
add_executable(campaign-extractor
    ${OPEN_RIVAL_CAMPAIGN_EXTRACTOR_EXTERNAL_SOURCES}
    ${OPEN_RIVAL_CAMPAIGN_EXTRACTOR_SOURCES}
    ${OPEN_RIVAL_CAMPAIGN_EXTRACTOR_INCLUDE_HEADERS}
    ${OPEN_RIVAL_CAMPAIGN_EXTRACTOR_EXTERNAL_INCLUDE_HEADERS}
)
target_include_directories(campaign-extractor PUBLIC
    ${OPEN_RIVAL_CAMPAIGN_EXTRACTOR_INCLUDE_DIRS}
    ${OPEN_RIVAL_CAMPAIGN_EXTRACTOR_EXTERNAL_DIRS}
)

target_link_libraries(campaign-extractor PRIVATE
    project_options
    project_warnings
)
