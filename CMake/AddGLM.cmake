#
# Adds a dependency on GLM to a project.
#
function(add_glm project_name)

    # Include directory
    target_include_directories(${project_name} PRIVATE
        "${LIBS_DIR}/glm-${GLM_VERSION}"
    )

endfunction()
