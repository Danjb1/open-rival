#
# Adds a dependency on SDL2_Image to a project.
#
function(add_sdl2_image project_name)

    # Detect 32- vs 64-bit systems
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_sdl2_image_lib_suffix "lib/x64")
    else()
        set(_sdl2_image_lib_suffix "lib/x86")
    endif()

    # Include directory
    target_include_directories(${project_name} PRIVATE
        "${LIBS_DIR}/SDL2_image-${SDL2_IMAGE_VERSION}/include"
    )

    # Library directory
    if(WIN32)
        target_link_directories(${project_name} PUBLIC
            "$<$<CONFIG:Debug>:${LIBS_DIR}/SDL2_image-${SDL2_IMAGE_VERSION}/${_sdl2_image_lib_suffix}>"
            "$<$<CONFIG:Release>:${LIBS_DIR}/SDL2_image-${SDL2_IMAGE_VERSION}/${_sdl2_image_lib_suffix}>"
        )
    else()
        message(STATUS "Operating system not (yet) supported!")
    endif()

    # Library filename
    target_link_libraries(${project_name} PRIVATE
        SDL2_image
    )

endfunction()
