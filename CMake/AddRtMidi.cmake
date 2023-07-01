#
# Adds a dependency on RtMidi to a project.
#
function(add_rtmidi project_name)

    # Detect 32- vs 64-bit systems
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_rtmidi_lib_suffix "build/x64")
    else()
        set(_rtmidi_lib_suffix "build/x86")
    endif()

    # Include directory
    target_include_directories(${project_name} PRIVATE
        ${LIBS_DIR}/rtmidi-${RTMIDI_VERSION}
    )

    # Library directory
    if(WIN32)
        target_link_directories(${project_name} PUBLIC
            $<$<CONFIG:Debug>:${LIBS_DIR}/rtmidi-${RTMIDI_VERSION}/${_rtmidi_lib_suffix}/Debug>
            $<$<CONFIG:Release>:${LIBS_DIR}/rtmidi-${RTMIDI_VERSION}/${_rtmidi_lib_suffix}/Release>
        )
    else()
        message(STATUS "Operating system not (yet) supported!")
    endif()

    # Library filenames
    target_link_libraries(${project_name} PRIVATE
        rtmidi
        winmm
    )

endfunction()
