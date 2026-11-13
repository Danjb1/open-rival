add_executable(image-extractor
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
)

target_link_libraries(image-extractor PRIVATE
    setup-lib
    project_warnings
)
