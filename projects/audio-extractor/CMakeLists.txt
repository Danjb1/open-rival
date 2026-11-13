add_executable(audio-extractor
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
)

target_link_libraries(audio-extractor PRIVATE
    setup-lib
    project_warnings
)
