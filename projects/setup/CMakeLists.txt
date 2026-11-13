################################################################################
# Setup library
# --------------
# Resource extraction / generation code, shared between the setup program and
# the standalone extraction tools.
################################################################################

set(SETUP_LIB_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/audio-extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/image-extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ImageGen.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/interface-extractor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/registry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/setup-utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/texture-builder.cpp
)

set(SETUP_LIB_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/audio-extractor.h
    ${CMAKE_CURRENT_LIST_DIR}/include/image-extractor.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ImageGen.h
    ${CMAKE_CURRENT_LIST_DIR}/include/interface-extractor.h
    ${CMAKE_CURRENT_LIST_DIR}/include/registry.h
    ${CMAKE_CURRENT_LIST_DIR}/include/setup-utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/texture-builder.h
)

add_library(setup-lib STATIC
    ${SETUP_LIB_SOURCES}
    ${SETUP_LIB_HEADERS}
)

target_include_directories(setup-lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(setup-lib
  PUBLIC
    Open-Rival-common
  PRIVATE
    project_warnings
)

# Use Unicode encoding on Windows (required by the registry functions).
# This is PRIVATE so it doesn't affect the standalone tools.
if (MSVC)
    target_compile_definitions(setup-lib PRIVATE UNICODE _UNICODE)
endif()

################################################################################
# Setup executable
################################################################################

add_executable(setup
    ${CMAKE_CURRENT_LIST_DIR}/src/Main.cpp
)

target_link_libraries(setup PRIVATE
    setup-lib
    project_warnings
)

# Use Unicode encoding on Windows
if (MSVC)
    target_compile_definitions(setup PRIVATE UNICODE _UNICODE)
endif()

# Copy "setup" folder to build folder
file(COPY "${CMAKE_SOURCE_DIR}/dist/setup" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
