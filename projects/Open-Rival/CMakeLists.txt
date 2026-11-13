################################################################################
# Source files
# -------------
# Paths are relative to this folder.
################################################################################

# BEGIN GENERATED FILE LIST (see tools/gen_project_files.py)
set(OPEN_RIVAL_SOURCES
    src/application/Application.cpp
    src/application/PathUtils.cpp
    src/application/ResourceLoader.cpp
    src/application/Resources.cpp
    src/application/State.cpp
    src/audio/AudioUtils.cpp
    src/audio/MidiContainer.cpp
    src/audio/MidiFile.cpp
    src/audio/MidiPlayer.cpp
    src/audio/MidsDecoder.cpp
    src/audio/SoundSource.cpp
    src/audio/Sounds.cpp
    src/audio/WaveFile.cpp
    src/audio/openal/OpenALAudioSystem.cpp
    src/commands/AttackCommand.cpp
    src/commands/GameCommand.cpp
    src/commands/GameCommandFactory.cpp
    src/commands/MoveCommand.cpp
    src/entity/Building.cpp
    src/entity/Decoration.cpp
    src/entity/Effect.cpp
    src/entity/Entity.cpp
    src/entity/EntityFactory.cpp
    src/entity/Palisade.cpp
    src/entity/Projectile.cpp
    src/entity/Unit.cpp
    src/entity/components/ArmorComponent.cpp
    src/entity/components/AttackComponent.cpp
    src/entity/components/BasicAnimationComponent.cpp
    src/entity/components/BuildingAnimationComponent.cpp
    src/entity/components/DeathEffectComponent.cpp
    src/entity/components/EffectAnimationComponent.cpp
    src/entity/components/EntityComponent.cpp
    src/entity/components/FacingComponent.cpp
    src/entity/components/FlyerComponent.cpp
    src/entity/components/HealthComponent.cpp
    src/entity/components/MouseHandlerComponent.cpp
    src/entity/components/MovementComponent.cpp
    src/entity/components/OwnerComponent.cpp
    src/entity/components/PassabilityComponent.cpp
    src/entity/components/PortraitComponent.cpp
    src/entity/components/ProjectileAnimationComponent.cpp
    src/entity/components/ProjectileMovementComponent.cpp
    src/entity/components/SeafarerComponent.cpp
    src/entity/components/SpriteComponent.cpp
    src/entity/components/UnitAnimationComponent.cpp
    src/entity/components/VoiceComponent.cpp
    src/entity/components/WalkerComponent.cpp
    src/entity/components/WallComponent.cpp
    src/game/Animations.cpp
    src/game/AttackDef.cpp
    src/game/AttackUtils.cpp
    src/game/BuildingDef.cpp
    src/game/EffectDef.cpp
    src/game/GameInterface.cpp
    src/game/GameState.cpp
    src/game/InventoryComponent.cpp
    src/game/MapUtils.cpp
    src/game/MousePicker.cpp
    src/game/PassabilityMap.cpp
    src/game/Pathfinding.cpp
    src/game/PlayerContext.cpp
    src/game/PlayerState.cpp
    src/game/ProjectileDef.cpp
    src/game/SpatialIndex.cpp
    src/game/Tile.cpp
    src/game/UnitDef.cpp
    src/game/World.cpp
    src/gfx/Camera.cpp
    src/gfx/Color.cpp
    src/gfx/Font.cpp
    src/gfx/Image.cpp
    src/gfx/Palette.cpp
    src/gfx/PaletteUtils.cpp
    src/gfx/RenderUtils.cpp
    src/gfx/Renderer.cpp
    src/gfx/Spritesheet.cpp
    src/gfx/Texture.cpp
    src/gfx/TextureAtlas.cpp
    src/gfx/opengl/Framebuffer.cpp
    src/gfx/opengl/GLRenderer.cpp
    src/gfx/opengl/GLUtils.cpp
    src/gfx/opengl/ShaderUtils.cpp
    src/gfx/opengl/Shaders.cpp
    src/gfx/opengl/renderable/AtlasRenderable.cpp
    src/gfx/opengl/renderable/BoxRenderable.cpp
    src/gfx/opengl/renderable/SpriteRenderable.cpp
    src/gfx/opengl/renderable/TextRenderable.cpp
    src/gfx/opengl/renderable/TextureRenderable.cpp
    src/gfx/opengl/renderer/CursorRenderer.cpp
    src/gfx/opengl/renderer/DragSelectRenderer.cpp
    src/gfx/opengl/renderer/EntityOverlayRenderer.cpp
    src/gfx/opengl/renderer/EntityRenderer.cpp
    src/gfx/opengl/renderer/FramebufferRenderer.cpp
    src/gfx/opengl/renderer/GameRenderer.cpp
    src/gfx/opengl/renderer/LobbyRenderer.cpp
    src/gfx/opengl/renderer/MapBorderRenderer.cpp
    src/gfx/opengl/renderer/MenuRenderer.cpp
    src/gfx/opengl/renderer/MenuTextRenderer.cpp
    src/gfx/opengl/renderer/TextRenderer.cpp
    src/gfx/opengl/renderer/TileRenderer.cpp
    src/gfx/opengl/renderer/UiRenderer.cpp
    src/input/Input.cpp
    src/lobby/LobbyState.cpp
    src/main/Main.cpp
    src/main/ProgramOptions.cpp
    src/net/ClientInfo.cpp
    src/net/Connection.cpp
    src/net/PacketFactory.cpp
    src/net/Server.cpp
    src/net/Socket.cpp
    src/net/WindowsNetUtils.cpp
    src/net/packet-handlers/AcceptPlayerPacketHandler.cpp
    src/net/packet-handlers/GameCommandPacketHandler.cpp
    src/net/packet-handlers/KickPlayerPacketHandler.cpp
    src/net/packet-handlers/LobbyWelcomePacketHandler.cpp
    src/net/packet-handlers/PacketHandler.cpp
    src/net/packet-handlers/RejectPlayerPacketHandler.cpp
    src/net/packet-handlers/RequestJoinPacketHandler.cpp
    src/net/packet-handlers/StartGamePacketHandler.cpp
    src/net/packets/AcceptPlayerPacket.cpp
    src/net/packets/GameCommandPacket.cpp
    src/net/packets/KickPlayerPacket.cpp
    src/net/packets/LobbyWelcomePacket.cpp
    src/net/packets/Packet.cpp
    src/net/packets/RejectPlayerPacket.cpp
    src/net/packets/RelayedPacket.cpp
    src/net/packets/RequestJoinPacket.cpp
    src/net/packets/StartGamePacket.cpp
    src/platform/unix/UnixTimeUtils.cpp
    src/platform/win32/WindowsSocket.cpp
    src/platform/win32/WindowsTimeUtils.cpp
    src/scenario/ScenarioBuilder.cpp
    src/scenario/ScenarioReader.cpp
    src/ui/Cursor.cpp
    src/utils/BinaryFileReader.cpp
    src/utils/BufferUtils.cpp
    src/utils/FileUtils.cpp
    src/utils/HashUtils.cpp
    src/utils/JsonUtils.cpp
    src/utils/LogUtils.cpp
    src/utils/MathUtils.cpp
    src/utils/MouseUtils.cpp
    src/utils/Rect.cpp
    src/utils/StringUtils.cpp
    src/utils/TimeUtils.cpp
    src/window/Window.cpp
    src/window/sdl/SDLWindowImpl.cpp
)

set(OPEN_RIVAL_HEADERS
    include/application/Application.h
    include/application/PathUtils.h
    include/application/ResourceLoader.h
    include/application/Resources.h
    include/application/State.h
    include/audio/AudioSystem.h
    include/audio/AudioUtils.h
    include/audio/MidiContainer.h
    include/audio/MidiFile.h
    include/audio/MidiPlayer.h
    include/audio/MidsDecoder.h
    include/audio/SoundSource.h
    include/audio/Sounds.h
    include/audio/WaveFile.h
    include/audio/openal/OpenALAudioSystem.h
    include/commands/AttackCommand.h
    include/commands/GameCommand.h
    include/commands/GameCommandFactory.h
    include/commands/MoveCommand.h
    include/entity/Building.h
    include/entity/Decoration.h
    include/entity/Effect.h
    include/entity/Entity.h
    include/entity/EntityFactory.h
    include/entity/EntityType.h
    include/entity/Palisade.h
    include/entity/Projectile.h
    include/entity/Unit.h
    include/entity/components/AnimationListener.h
    include/entity/components/ArmorComponent.h
    include/entity/components/AttackComponent.h
    include/entity/components/AttackListener.h
    include/entity/components/BasicAnimationComponent.h
    include/entity/components/BuildingAnimationComponent.h
    include/entity/components/DeathEffectComponent.h
    include/entity/components/EffectAnimationComponent.h
    include/entity/components/EntityComponent.h
    include/entity/components/FacingComponent.h
    include/entity/components/FlyerComponent.h
    include/entity/components/HealthComponent.h
    include/entity/components/MouseHandlerComponent.h
    include/entity/components/MovementComponent.h
    include/entity/components/MovementListener.h
    include/entity/components/OwnerComponent.h
    include/entity/components/PassabilityComponent.h
    include/entity/components/PortraitComponent.h
    include/entity/components/ProjectileAnimationComponent.h
    include/entity/components/ProjectileMovementComponent.h
    include/entity/components/SeafarerComponent.h
    include/entity/components/SpriteComponent.h
    include/entity/components/UnitAnimationComponent.h
    include/entity/components/VoiceComponent.h
    include/entity/components/WalkerComponent.h
    include/entity/components/WallComponent.h
    include/game/Animations.h
    include/game/AttackDef.h
    include/game/AttackUtils.h
    include/game/BuildingDef.h
    include/game/BuildingType.h
    include/game/EffectDef.h
    include/game/GameInterface.h
    include/game/GameState.h
    include/game/InventoryComponent.h
    include/game/MapUtils.h
    include/game/MousePicker.h
    include/game/PassabilityMap.h
    include/game/Pathfinding.h
    include/game/PlayerContext.h
    include/game/PlayerState.h
    include/game/ProjectileDef.h
    include/game/Race.h
    include/game/SpatialIndex.h
    include/game/Tile.h
    include/game/UnitDef.h
    include/game/UnitType.h
    include/game/World.h
    include/game/WorldStore.h
    include/gfx/Camera.h
    include/gfx/Color.h
    include/gfx/Font.h
    include/gfx/Image.h
    include/gfx/Palette.h
    include/gfx/PaletteUtils.h
    include/gfx/RenderConstants.h
    include/gfx/RenderUtils.h
    include/gfx/Renderer.h
    include/gfx/Spritesheet.h
    include/gfx/Texture.h
    include/gfx/TextureAtlas.h
    include/gfx/opengl/Framebuffer.h
    include/gfx/opengl/GLRenderer.h
    include/gfx/opengl/GLUtils.h
    include/gfx/opengl/GLWrapper.h
    include/gfx/opengl/ShaderUtils.h
    include/gfx/opengl/Shaders.h
    include/gfx/opengl/renderable/AtlasRenderable.h
    include/gfx/opengl/renderable/BoxRenderable.h
    include/gfx/opengl/renderable/SpriteRenderable.h
    include/gfx/opengl/renderable/TextRenderable.h
    include/gfx/opengl/renderable/TextureRenderable.h
    include/gfx/opengl/renderer/CursorRenderer.h
    include/gfx/opengl/renderer/DragSelectRenderer.h
    include/gfx/opengl/renderer/EntityOverlayRenderer.h
    include/gfx/opengl/renderer/EntityRenderer.h
    include/gfx/opengl/renderer/FramebufferRenderer.h
    include/gfx/opengl/renderer/GameRenderer.h
    include/gfx/opengl/renderer/LobbyRenderer.h
    include/gfx/opengl/renderer/MapBorderRenderer.h
    include/gfx/opengl/renderer/MenuRenderer.h
    include/gfx/opengl/renderer/MenuTextRenderer.h
    include/gfx/opengl/renderer/TextRenderer.h
    include/gfx/opengl/renderer/TileRenderer.h
    include/gfx/opengl/renderer/UiRenderer.h
    include/input/Input.h
    include/lobby/LobbyState.h
    include/main/ProgramOptions.h
    include/net/ClientInfo.h
    include/net/Connection.h
    include/net/NetUtils.h
    include/net/PacketFactory.h
    include/net/Server.h
    include/net/Socket.h
    include/net/packet-handlers/AcceptPlayerPacketHandler.h
    include/net/packet-handlers/GameCommandPacketHandler.h
    include/net/packet-handlers/KickPlayerPacketHandler.h
    include/net/packet-handlers/LobbyWelcomePacketHandler.h
    include/net/packet-handlers/PacketHandler.h
    include/net/packet-handlers/RejectPlayerPacketHandler.h
    include/net/packet-handlers/RequestJoinPacketHandler.h
    include/net/packet-handlers/StartGamePacketHandler.h
    include/net/packets/AcceptPlayerPacket.h
    include/net/packets/GameCommandPacket.h
    include/net/packets/KickPlayerPacket.h
    include/net/packets/LobbyWelcomePacket.h
    include/net/packets/Packet.h
    include/net/packets/RejectPlayerPacket.h
    include/net/packets/RelayedPacket.h
    include/net/packets/RequestJoinPacket.h
    include/net/packets/StartGamePacket.h
    include/platform/WindowsWrapper.h
    include/scenario/ScenarioBuilder.h
    include/scenario/ScenarioData.h
    include/scenario/ScenarioReader.h
    include/ui/Cursor.h
    include/utils/BinaryFileReader.h
    include/utils/BufferUtils.h
    include/utils/CollectionUtils.h
    include/utils/ConfigUtils.h
    include/utils/EntityUtils.h
    include/utils/EnumUtils.h
    include/utils/FileUtils.h
    include/utils/HashUtils.h
    include/utils/IterationGuard.h
    include/utils/JsonFwd.h
    include/utils/JsonUtils.h
    include/utils/LogUtils.h
    include/utils/MathUtils.h
    include/utils/MouseUtils.h
    include/utils/Rect.h
    include/utils/SDLWrapper.h
    include/utils/StringUtils.h
    include/utils/TimeUtils.h
    include/window/Window.h
    include/window/sdl/SDLWindowImpl.h
)
# END GENERATED FILE LIST

# Low-level utilities that are shared with the setup / extraction tools.
# These go in Open-Rival-common, so they must not depend on anything else in
# the game.
set(OPEN_RIVAL_COMMON_SOURCES
    src/gfx/Image.cpp
    src/gfx/Palette.cpp
    src/utils/BinaryFileReader.cpp
    src/utils/FileUtils.cpp
    src/utils/LogUtils.cpp
    src/utils/MathUtils.cpp
    src/utils/StringUtils.cpp
)
set(OPEN_RIVAL_COMMON_HEADERS
    include/gfx/Image.h
    include/gfx/Palette.h
    include/utils/BinaryFileReader.h
    include/utils/ConfigUtils.h
    include/utils/FileUtils.h
    include/utils/LogUtils.h
    include/utils/MathUtils.h
    include/utils/StringUtils.h
)
list(REMOVE_ITEM OPEN_RIVAL_SOURCES ${OPEN_RIVAL_COMMON_SOURCES})
list(REMOVE_ITEM OPEN_RIVAL_HEADERS ${OPEN_RIVAL_COMMON_HEADERS})

# Files under `main` belong to the executable; everything else goes in the
# library (which is shared with the unit tests).
set(OPEN_RIVAL_MAIN_SOURCES ${OPEN_RIVAL_SOURCES})
set(OPEN_RIVAL_MAIN_HEADERS ${OPEN_RIVAL_HEADERS})
list(FILTER OPEN_RIVAL_MAIN_SOURCES INCLUDE REGEX "^src/main/")
list(FILTER OPEN_RIVAL_MAIN_HEADERS INCLUDE REGEX "^include/main/")
list(FILTER OPEN_RIVAL_SOURCES EXCLUDE REGEX "^src/main/")
list(FILTER OPEN_RIVAL_HEADERS EXCLUDE REGEX "^include/main/")

# Docs (not required for compilation, but helpful to have at hand)
set(OPEN_RIVAL_DOCS
    "${CMAKE_SOURCE_DIR}/dist/docs/changelog.txt"
    "${CMAKE_SOURCE_DIR}/docs/TODO.md"
)

# Extra project files (not required for compilation, but helpful to have at hand)
set(OPEN_RIVAL_EXTRA_FILES
    "${CMAKE_SOURCE_DIR}/projects/.clang-format"
    "${CMAKE_CURRENT_LIST_DIR}/Version.h.in"
)

# Generate Version.h from the project version (see the root CMakeLists.txt).
# This goes in the build folder, so the generated file is never committed.
set(OPEN_RIVAL_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/Version.h.in"
    "${OPEN_RIVAL_GENERATED_INCLUDE_DIR}/Version.h"
)

# Include directories
set(OPEN_RIVAL_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_LIST_DIR}/"
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

# C++ resource files
set(OPEN_RIVAL_RESOURCES
    "${CMAKE_CURRENT_LIST_DIR}/Open-Rival.rc"
    "${CMAKE_CURRENT_LIST_DIR}/resource.h"
)

# Resources folder
set(OPEN_RIVAL_RES_SRC_DIR "${CMAKE_SOURCE_DIR}/dist/res")

# Config file
set(OPEN_RIVAL_CONFIG_SRC "${CMAKE_SOURCE_DIR}/dist/config.json")

# Organise files into folders (filters)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "Source Files" FILES ${OPEN_RIVAL_SOURCES} ${OPEN_RIVAL_MAIN_SOURCES} ${OPEN_RIVAL_COMMON_SOURCES})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/include PREFIX "Source Files" FILES ${OPEN_RIVAL_HEADERS} ${OPEN_RIVAL_MAIN_HEADERS} ${OPEN_RIVAL_COMMON_HEADERS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} PREFIX "Resources" FILES ${OPEN_RIVAL_RESOURCES})
source_group("Docs" FILES ${OPEN_RIVAL_DOCS})

# Find dependencies
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(rtmidi REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(spdlog REQUIRED)

################################################################################
# Common library
# ---------------
# Low-level utilities that are shared between the game and the setup /
# extraction tools.
################################################################################

add_library(Open-Rival-common STATIC
    ${OPEN_RIVAL_COMMON_SOURCES}
    ${OPEN_RIVAL_COMMON_HEADERS}
)

target_include_directories(Open-Rival-common PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
    "${OPEN_RIVAL_GENERATED_INCLUDE_DIR}"
)

# These are PUBLIC because our headers expose these libraries' types
target_link_libraries(Open-Rival-common
  PUBLIC
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    project_options
  PRIVATE
    project_warnings
)

################################################################################
# Game library
# -------------
# Contains all of the game code, so that it can be shared between the
# executable and the unit tests.
################################################################################

add_library(Open-Rival-lib STATIC
    ${OPEN_RIVAL_SOURCES}
    ${OPEN_RIVAL_HEADERS}
)

# Add the directories and headers of the project
target_include_directories(Open-Rival-lib PUBLIC
    ${OPEN_RIVAL_INCLUDE_DIRECTORIES}
)

# Link dependencies.
# These are PUBLIC because our headers expose many of these libraries' types.
target_link_libraries(Open-Rival-lib
  PUBLIC
    Open-Rival-common
    Freetype::Freetype
    GLEW::glew
    glm::glm
    $<$<CONFIG:Debug>:imgui::imgui> # debug only
    OpenAL::OpenAL
    OpenGL::GL
    OpenGL::GLU
    RtMidi::rtmidi
    SDL3::SDL3
    $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
    project_options
    # Windows-only libraries (multimedia timers, sockets)
    $<$<PLATFORM_ID:Windows>:winmm>
    $<$<PLATFORM_ID:Windows>:Ws2_32>
  PRIVATE
    project_warnings
)

# Add macro definitions.
# These are PUBLIC because they affect the contents of our headers.
target_compile_definitions(Open-Rival-lib PUBLIC
    # Define DEBUG and ENABLE_IMGUI macros in debug builds
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Debug>:ENABLE_IMGUI>
)

# Pre-compiled headers.
# Only include headers that rarely change (standard library, third-party
# libraries), since any change to these triggers a full rebuild.
target_precompile_headers(Open-Rival-lib PRIVATE
    # Standard library
    <cstdint>
    <functional>
    <memory>
    <optional>
    <stdexcept>
    <string>
    <unordered_map>
    <unordered_set>
    <vector>
    # Third-party libraries
    <glm/glm.hpp>
    <nlohmann/json.hpp>
    <spdlog/spdlog.h>
    # NOTE: <windows.h> is deliberately excluded; WindowsWrapper.h undefines
    # `near` and `far`, which breaks other Windows headers included later.
)

################################################################################
# Executable
################################################################################

add_executable(Open-Rival
    ${OPEN_RIVAL_MAIN_SOURCES}
    ${OPEN_RIVAL_MAIN_HEADERS}
    ${OPEN_RIVAL_RESOURCES}
    ${OPEN_RIVAL_DOCS}
    ${OPEN_RIVAL_EXTRA_FILES}
)

# Set WIN32_EXECUTABLE for Release builds.
# This hides the console and calls `WinMain` instead of `main.
# This is ignored for non-Windows platforms.
set_target_properties(Open-Rival PROPERTIES
    WIN32_EXECUTABLE $<CONFIG:Release>
)

target_link_libraries(Open-Rival PRIVATE
    Open-Rival-lib
    project_warnings
)

################################################################################
# Resources
# ----------
# These are copied to the build folder, and changes to these files will trigger
# a new CMake build.
################################################################################

# Find all resources
file(GLOB_RECURSE RESOURCE_LIST "${OPEN_RIVAL_RES_SRC_DIR}/*")

# Create a custom command to copy resources
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/res
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${OPEN_RIVAL_RES_SRC_DIR} ${CMAKE_CURRENT_BINARY_DIR}/res
    DEPENDS ${RESOURCE_LIST}
    COMMENT "Copying resources to build directory"
)

# Create a custom target that depends on the custom command
add_custom_target(copy_assets ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/res)

# Ensure the custom target runs before Open-Rival
add_dependencies(Open-Rival copy_assets)

################################################################################
# Extra files
# ------------
# These are copied to the build folder when CMake runs, but changes to these
# files will not trigger a new CMake build.
#
# Note that DLLs are copied next to the executable by vcpkg after each build.
################################################################################

# Copy config file to build folder (if not present)
if(NOT(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/config.json))
    file(COPY ${OPEN_RIVAL_CONFIG_SRC} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
