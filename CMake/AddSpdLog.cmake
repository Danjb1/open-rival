#
# Adds a dependency on spdlog to a project.
#
function(add_spdlog project_name)

    # Detect 32- vs 64-bit systems
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_spdlog_lib_suffix "build/x64")
    else()
        set(_spdlog_lib_suffix "build/Win32")
    endif()

    # Include directory
    target_include_directories(${project_name} PRIVATE
        "${LIBS_DIR}/spdlog-${SPDLOG_VERSION}/include"
    )
    
    # Library directory
    if(WIN32)
        target_link_directories(${project_name} PUBLIC
            "$<$<CONFIG:Debug>:${LIBS_DIR}/spdlog-${SPDLOG_VERSION}/${_spdlog_lib_suffix}/Debug>"
            "$<$<CONFIG:Release>:${LIBS_DIR}/spdlog-${SPDLOG_VERSION}/${_spdlog_lib_suffix}/Release>"
        )
    else()
        message(STATUS "Operating system not (yet) supported!")
    endif()

    # Library filename
    target_link_libraries(${project_name} PRIVATE
        $<$<CONFIG:Debug>:spdlogd>
        $<$<CONFIG:Release>:spdlog>
    )

endfunction()
