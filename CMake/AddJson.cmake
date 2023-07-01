#
# Adds a dependency on JSON to a project.
#
function(add_json project_name)

    # Include directory
    target_include_directories(${project_name} PRIVATE
        ${LIBS_DIR}/json-${JSON_VERSION}/include
    )

endfunction()
