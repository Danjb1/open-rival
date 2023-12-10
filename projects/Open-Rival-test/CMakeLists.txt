cmake_minimum_required (VERSION 3.16)

# Pull in auto-generated list of source/header files
set(OPEN_RIVAL_DIR ${CMAKE_SOURCE_DIR}/projects/Open-Rival)
include(${CMAKE_SOURCE_DIR}/CMake/OpenRivalFiles.cmake)

# Remove Open-Rival sources that we don't need or that we are replacing
list(REMOVE_ITEM OPEN_RIVAL_SOURCES
    ${OPEN_RIVAL_DIR}/src/application/Resources.cpp
    ${OPEN_RIVAL_DIR}/src/application/Window.cpp
    ${OPEN_RIVAL_DIR}/src/audio/AudioSystem.cpp
    ${OPEN_RIVAL_DIR}/src/audio/AudioUtils.cpp
    ${OPEN_RIVAL_DIR}/src/audio/MidiPlayer.cpp
    ${OPEN_RIVAL_DIR}/src/entity/EntityRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderable/AtlasRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderable/BoxRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderable/SpriteRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderable/TextRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderable/TextureRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/DragSelectRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/EntityRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/FramebufferRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/GameRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/MapBorderRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/MenuTextRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/TextRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/TileRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/UiRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Font.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Framebuffer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/PaletteUtils.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Shaders.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/ShaderUtils.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Texture.cpp
    ${OPEN_RIVAL_DIR}/src/ui/CursorRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/ui/MenuRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/main/Main.cpp
)

# Replacement Headers
set(OPEN_RIVAL_REPLACEMENT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/audio/AudioSystem.h
    ${CMAKE_CURRENT_LIST_DIR}/include/gfx/renderer/GameRenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/gfx/renderer/MenuTextRenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/gl/glew.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ui/MenuRenderer.h
)

# Test Framework
set(OPEN_RIVAL_TEST_FRAMEWORK_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/catch2/catch.h
)

# Test Headers
set(OPEN_RIVAL_TEST_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/mocks/MockSDL.h
)

# Test Sources
set(OPEN_RIVAL_TEST_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/main/Main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockAudioSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockAudioUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockGameRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockGLEW.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockFont.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockFramebuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockFreeType.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockMenuRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockMenuTextRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockResources.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockSDL.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockSDLImage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockShaders.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockSpriteRenderable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockTextRenderable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockTexture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/mocks/MockWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestAnimationComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestApplication.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestCamera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestEntity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestMapUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestMousePicker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestRenderUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestSpritesheet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/tests/TestWalkerComponent.cpp
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

# Dependencies (these ones are pretty lightweight so we just use them directly)
add_glm(Open-Rival-test)
add_json(Open-Rival-test)
add_spdlog(Open-Rival-test)

# Adds the libraries it must link against to find the symbols
target_link_libraries(Open-Rival-test PRIVATE
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
