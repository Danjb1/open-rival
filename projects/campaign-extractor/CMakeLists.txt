add_executable(campaign-extractor
    ${CMAKE_CURRENT_LIST_DIR}/campaign-extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/campaign-extractor.h
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
)

target_include_directories(campaign-extractor PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(campaign-extractor PRIVATE
    Open-Rival-common
    project_warnings
)
