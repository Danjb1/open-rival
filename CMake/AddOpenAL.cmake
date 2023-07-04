#
# Adds a dependency on OpenAL to a project.
#
function(add_openal project_name)

    # Detect 32- vs 64-bit systems
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_openal_lib_suffix "build/x64")
    else()
        set(_openal_lib_suffix "build/x86")
    endif()

    # Include directory
    target_include_directories(${project_name} PRIVATE
        ${LIBS_DIR}/openal-soft-${OPENAL_VERSION}/include
    )

    # Library directory
    if(WIN32)
        target_link_directories(${project_name} PUBLIC
            $<$<CONFIG:Debug>:${LIBS_DIR}/openal-soft-${OPENAL_VERSION}/${_openal_lib_suffix}/Debug>
            $<$<CONFIG:Release>:${LIBS_DIR}/openal-soft-${OPENAL_VERSION}/${_openal_lib_suffix}/Release>
        )
    else()
        message(STATUS "Operating system not (yet) supported!")
    endif()

    # Library filename
    target_link_libraries(${project_name} PRIVATE
        OpenAL32
    )

endfunction()
