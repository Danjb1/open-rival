add_executable(interface-extractor
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
)

target_link_libraries(interface-extractor PRIVATE
    setup-lib
    project_warnings
)
