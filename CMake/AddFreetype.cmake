#
# Adds a dependency on Freetype to a project.
#
function(add_freetype project_name)

    # Detect 32- vs 64-bit systems
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_freetype_lib_suffix "objs/x64")
    else()
        set(_freetype_lib_suffix "objs/Win32")
    endif()

    # Include directory
    target_include_directories(${project_name} PRIVATE
        ${LIBS_DIR}/freetype-${FREETYPE_VERSION}/include
    )

    # Library directory
    if(WIN32)
        target_link_directories(${project_name} PUBLIC
            $<$<CONFIG:Debug>:${LIBS_DIR}/freetype-${FREETYPE_VERSION}/${_freetype_lib_suffix}/Debug>
            $<$<CONFIG:Release>:${LIBS_DIR}/freetype-${FREETYPE_VERSION}/${_freetype_lib_suffix}/ Release>
        )
    else()
        message(STATUS "Operating system not (yet) supported!")
    endif()

    # Library filename
    target_link_libraries(${project_name} PRIVATE
        freetype
    )

endfunction()
