cmake_minimum_required (VERSION 3.16)

# --- Retrieve Open-Rival files ---
# Pull in auto-generated list of source/header files
set(OPEN_RIVAL_DIR ${CMAKE_SOURCE_DIR}/projects/Open-Rival)
include(${CMAKE_SOURCE_DIR}/CMake/OpenRivalFiles.cmake)

# --- Exclude some .cpp files ---
# Remove Open-Rival sources that we don't need or that we are replacing.
# In particular, all OpenGL files should be removed!
list(REMOVE_ITEM OPEN_RIVAL_SOURCES
    ${OPEN_RIVAL_DIR}/src/application/Window.cpp
    ${OPEN_RIVAL_DIR}/src/audio/AudioSystem.cpp
    ${OPEN_RIVAL_DIR}/src/audio/AudioUtils.cpp
    ${OPEN_RIVAL_DIR}/src/audio/MidiPlayer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/Framebuffer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/GLRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/GLUtils.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/ShaderUtils.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/Shaders.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderable/AtlasRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderable/BoxRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderable/SpriteRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderable/TextRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderable/TextureRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/CursorRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/DragSelectRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/EntityOverlayRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/EntityRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/FramebufferRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/GameRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/LobbyRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/MapBorderRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/MenuRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/MenuTextRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/TextRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/TileRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/opengl/renderer/UiRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Font.cpp
    ${OPEN_RIVAL_DIR}/src/main/Main.cpp
)

# --- Replacement Headers ---
# These are test-only header files intended to replace their "real" counterparts.
set(OPEN_RIVAL_REPLACEMENT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/audio/AudioSystem.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ui/MenuRenderer.h
)

# --- Test Framework ---
set(OPEN_RIVAL_TEST_FRAMEWORK_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/catch2/catch.h
)

# --- Test Headers ---
# These are header files that exist only in the test project.
set(OPEN_RIVAL_TEST_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/mocks/MockSDL.h
    ${CMAKE_CURRENT_LIST_DIR}/include/utils/EntityTestUtils.h
)

# --- Test Sources ---
# These are .cpp files that exist only in the test project,
# including the tests themselves.
set(OPEN_RIVAL_TEST_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/main/Main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockAudioSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockAudioUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockFont.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockFreeType.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockSDL.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestAnimationComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestApplication.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestAttackComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestCamera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestEntity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestMapUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestMousePicker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestRenderUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestSpritesheet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestWalkerComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/EntityTestUtils.cpp
)

# We include the test includes first so that they take precedence
set(OPEN_RIVAL_TEST_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${OPEN_RIVAL_DIR}/include
)

set(OPEN_RIVAL_TEST_LIBARY_INCLUDES
    # We include some library headers because we need the definitions, but we
    # don't actually link with the library. This works fine until we try to use
    # the library functionality, at which point we need to provide mock
    # implementations.
    ${LIBS_DIR}/freetype-${FREETYPE_VERSION}/include
    ${LIBS_DIR}/openal-soft-${OPENAL_VERSION}/include
    ${LIBS_DIR}/SDL2-${SDL2_VERSION}/include
    ${LIBS_DIR}/SDL2_image-${SDL2_IMAGE_VERSION}/include
    ${LIBS_DIR}/rtmidi-${RTMIDI_VERSION}
)

# Organise files into folders (filters)
source_group(TREE ${OPEN_RIVAL_DIR}/src PREFIX "Open-Rival Sources" FILES ${OPEN_RIVAL_SOURCES})
source_group(TREE ${OPEN_RIVAL_DIR}/include PREFIX "Open-Rival Sources" FILES ${OPEN_RIVAL_HEADERS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/include PREFIX "Test Framework" FILES ${OPEN_RIVAL_TEST_FRAMEWORK_HEADERS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/include PREFIX "Replacement Headers" FILES ${OPEN_RIVAL_REPLACEMENT_HEADERS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "Test Sources" FILES ${OPEN_RIVAL_TEST_SOURCES})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/include PREFIX "Test Sources" FILES ${OPEN_RIVAL_TEST_HEADERS})

# Creates the executable including all the sources and resources
add_executable(Open-Rival-test
    ${OPEN_RIVAL_SOURCES}
    ${OPEN_RIVAL_HEADERS}
    ${OPEN_RIVAL_REPLACEMENT_HEADERS}
    ${OPEN_RIVAL_TEST_FRAMEWORK_HEADERS}
    ${OPEN_RIVAL_TEST_SOURCES}
    ${OPEN_RIVAL_TEST_HEADERS}
)

# Add the directories and headers of the project
target_include_directories(Open-Rival-test PRIVATE
    ${OPEN_RIVAL_TEST_INCLUDE_DIRECTORIES}
    ${OPEN_RIVAL_TEST_LIBARY_INCLUDES}
)

# Find dependencies (these ones are pretty lightweight so we just use them directly)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# Adds the libraries it must link against to find the symbols
target_link_libraries(Open-Rival-test PRIVATE
    glm::glm
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    winmm
    Ws2_32
    project_options
    project_warnings
)

# Pre-compiled headers
target_precompile_headers(Open-Rival-test PRIVATE
    <cstdint>
    <memory>
    <stdexcept>
    <string>
    <vector>
)
