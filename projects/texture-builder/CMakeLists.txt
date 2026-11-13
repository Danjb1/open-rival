add_executable(texture-builder
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
)

target_link_libraries(texture-builder PRIVATE
    setup-lib
    project_warnings
)
