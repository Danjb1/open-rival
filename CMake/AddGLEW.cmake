#
# Adds a dependency on GLEW to a project.
#
function(add_glew project_name)

    # Detect 32- vs 64-bit systems
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_glew_lib_suffix "x64")
    else()
        set(_glew_lib_suffix "Win32")
    endif()

    # Include directory
    target_include_directories(${project_name} PRIVATE
        "${LIBS_DIR}/glew-${GLEW_VERSION}/include"
    )

    # Library directory
    if(WIN32)
        target_link_directories(${project_name} PUBLIC
            "$<$<CONFIG:Debug>:${LIBS_DIR}/glew-${GLEW_VERSION}/lib/Debug/${_glew_lib_suffix}>"
            "$<$<CONFIG:Release>:${LIBS_DIR}/glew-${GLEW_VERSION}/lib/Release/${_glew_lib_suffix}>"
        )
    else()
        message(STATUS "Operating system not (yet) supported!")
    endif()

    # Library filenames
    target_link_libraries(${project_name} PRIVATE
        $<$<CONFIG:Debug>:glew32sd>
        $<$<CONFIG:Release>:glew32s>
        glu32
        opengl32
    )

endfunction()
