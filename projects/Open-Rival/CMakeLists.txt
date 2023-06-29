cmake_minimum_required (VERSION 3.16)

# Dependency versions
set(FREETYPE_VERSION "2.13.0" CACHE STRING "FreeType version number")
set(GLEW_VERSION "2.1.0" CACHE STRING "glew version number")
set(GLM_VERSION "0.9.9.8" CACHE STRING "GLM version number")
set(JSON_VERSION "3.11.2" CACHE STRING "JSON version number")
set(OPENAL_VERSION "1.23.1" CACHE STRING "OpenAL version number")
set(RTMIDI_VERSION "5.0.0" CACHE STRING "RtMidi version number")
set(SDL2_VERSION "2.28.0" CACHE STRING "SDL2 version number")
set(SDL2_IMAGE_VERSION "2.6.3" CACHE STRING "SDL2_Image version number")

set(OPEN_RIVAL_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/commands/GameCommandFactory.cpp
    #${CMAKE_CURRENT_LIST_DIR}/src/gfx/BoxRenderable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/gfx/TextureRenderable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/lobby/LobbyState.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/ClientInfo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/Connection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/PacketFactory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/Server.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/Socket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/WindowsNetUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packet-handlers/AcceptPlayerPacketHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packet-handlers/GameCommandPacketHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packet-handlers/KickPlayerPacketHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packet-handlers/LobbyWelcomePacketHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packet-handlers/PacketHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packet-handlers/RejectPlayerPacketHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packet-handlers/RequestJoinPacketHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packet-handlers/StartGamePacketHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packets/AcceptPlayerPacket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packets/GameCommandPacket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packets/KickPlayerPacket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packets/LobbyWelcomePacket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packets/Packet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packets/RejectPlayerPacket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packets/RelayedPacket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packets/RequestJoinPacket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/packets/StartGamePacket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/platform/unix/UnixTimeUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/platform/win32/WindowsSocket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/platform/win32/WindowsTimeUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ui/CursorRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ui/MenuRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/BufferUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Animations.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ApplicationContext.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/AtlasRenderable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/AudioSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/AudioUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/BinaryFileReader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Building.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/BuildingAnimationComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/BuildingDef.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/BuildingPropsComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Camera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Color.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Cursor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Entity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/EntityComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/EntityFactory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/EntityRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/FacingComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/FileUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/FlyerComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Font.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Framebuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/FramebufferRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GameCommand.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GameInterface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GameRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GameState.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/GLUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Image.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/InputUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/InventoryComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/JsonUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MapBorderRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MapUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MathUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MenuTextRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MidiContainer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MidiFile.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MidiPlayer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MidsDecoder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MouseHandlerComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MousePicker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MouseUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MoveCommand.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MovementComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/OwnerComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Palette.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PaletteUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PassabilityComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Pathfinding.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PathUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PlayerContext.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PlayerState.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PortraitComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ProgramOptions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Rect.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/RenderUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Resources.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ScenarioBuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ScenarioReader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SeafarerComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Shaders.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ShaderUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Sounds.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SoundSource.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SpriteComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SpriteRenderable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Spritesheet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/State.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/StringUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TextRenderable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TextRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TextureAtlas.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Tile.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TileRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TimeUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/UiRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/UnitAnimationComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/UnitDef.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/UnitPropsComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/VoiceComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/WalkerComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/WallComponent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/WaveFile.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/World.cpp
)

set(OPEN_RIVAL_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/commands/GameCommandFactory.h
    #${CMAKE_CURRENT_LIST_DIR}/include/gfx/BoxRenderable.h
    ${CMAKE_CURRENT_LIST_DIR}/include/gfx/GlewWrapper.h
    ${CMAKE_CURRENT_LIST_DIR}/include/gfx/TextureRenderable.h
    ${CMAKE_CURRENT_LIST_DIR}/include/lobby/LobbyState.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/ClientInfo.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/Connection.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/NetUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/PacketFactory.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/Server.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/Socket.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packet-handlers/AcceptPlayerPacketHandler.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packet-handlers/GameCommandPacketHandler.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packet-handlers/KickPlayerPacketHandler.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packet-handlers/LobbyWelcomePacketHandler.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packet-handlers/PacketHandler.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packet-handlers/RejectPlayerPacketHandler.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packet-handlers/RequestJoinPacketHandler.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packet-handlers/StartGamePacketHandler.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packets/AcceptPlayerPacket.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packets/GameCommandPacket.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packets/KickPlayerPacket.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packets/LobbyWelcomePacket.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packets/Packet.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packets/RejectPlayerPacket.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packets/RelayedPacket.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packets/RequestJoinPacket.h
    ${CMAKE_CURRENT_LIST_DIR}/include/net/packets/StartGamePacket.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ui/CursorRenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ui/MenuRenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/utils/BufferUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Animations.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Application.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ApplicationContext.h
    ${CMAKE_CURRENT_LIST_DIR}/include/AtlasRenderable.h
    ${CMAKE_CURRENT_LIST_DIR}/include/AudioSystem.h
    ${CMAKE_CURRENT_LIST_DIR}/include/AudioUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/BinaryFileReader.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Building.h
    ${CMAKE_CURRENT_LIST_DIR}/include/BuildingAnimationComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/BuildingDef.h
    ${CMAKE_CURRENT_LIST_DIR}/include/BuildingPropsComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Camera.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Color.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ConfigUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Cursor.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Entity.h
    ${CMAKE_CURRENT_LIST_DIR}/include/EntityComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/EntityFactory.h
    ${CMAKE_CURRENT_LIST_DIR}/include/EntityRenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/EntityUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/EnumUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/FacingComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/FileUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/FlyerComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Font.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Framebuffer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/FramebufferRenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/GameCommand.h
    ${CMAKE_CURRENT_LIST_DIR}/include/GameInterface.h
    ${CMAKE_CURRENT_LIST_DIR}/include/GameRenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/GameState.h
    ${CMAKE_CURRENT_LIST_DIR}/include/GLUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Image.h
    ${CMAKE_CURRENT_LIST_DIR}/include/InputUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/InventoryComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/JsonUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MapBorderRenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MapUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MathUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MenuTextRenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MidiContainer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MidiFile.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MidiPlayer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MidsDecoder.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MouseHandlerComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MousePicker.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MouseUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MoveCommand.h
    ${CMAKE_CURRENT_LIST_DIR}/include/MovementComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/OwnerComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Palette.h
    ${CMAKE_CURRENT_LIST_DIR}/include/PaletteUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/PassabilityComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Pathfinding.h
    ${CMAKE_CURRENT_LIST_DIR}/include/PathUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/PlayerContext.h
    ${CMAKE_CURRENT_LIST_DIR}/include/PlayerState.h
    ${CMAKE_CURRENT_LIST_DIR}/include/PortraitComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ProgramOptions.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Race.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Rect.h
    ${CMAKE_CURRENT_LIST_DIR}/include/RenderUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Resources.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ScenarioBuilder.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ScenarioData.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ScenarioReader.h
    ${CMAKE_CURRENT_LIST_DIR}/include/SDLWrapper.h
    ${CMAKE_CURRENT_LIST_DIR}/include/SeafarerComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Shaders.h
    ${CMAKE_CURRENT_LIST_DIR}/include/ShaderUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Sounds.h
    ${CMAKE_CURRENT_LIST_DIR}/include/SoundSource.h
    ${CMAKE_CURRENT_LIST_DIR}/include/SpriteComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/SpriteRenderable.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Spritesheet.h
    ${CMAKE_CURRENT_LIST_DIR}/include/State.h
    ${CMAKE_CURRENT_LIST_DIR}/include/StringUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/TextRenderable.h
    ${CMAKE_CURRENT_LIST_DIR}/include/TextRenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Texture.h
    ${CMAKE_CURRENT_LIST_DIR}/include/TextureAtlas.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Tile.h
    ${CMAKE_CURRENT_LIST_DIR}/include/TileRenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/TimeUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/UiRenderer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Unit.h
    ${CMAKE_CURRENT_LIST_DIR}/include/UnitAnimationComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/UnitDef.h
    ${CMAKE_CURRENT_LIST_DIR}/include/UnitPropsComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/VoiceComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/WalkerComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/WallComponent.h
    ${CMAKE_CURRENT_LIST_DIR}/include/WaveFile.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Window.h
    ${CMAKE_CURRENT_LIST_DIR}/include/World.h
)

# Not required for compilation, but helpful to have at hand
set(OPEN_RIVAL_DOCS
    ${CMAKE_SOURCE_DIR}/dist/docs/changelog.txt
    ${CMAKE_SOURCE_DIR}/docs/TODO.md
)

# Not required for compilation, but helpful to have at hand
set(OPEN_RIVAL_EXTRA_FILES
    ${CMAKE_SOURCE_DIR}/projects/.clang-format
)

set(OPEN_RIVAL_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_LIST_DIR}/
    ${CMAKE_CURRENT_LIST_DIR}/include
)

set(OPEN_RIVAL_RESOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Open-Rival.rc
    ${CMAKE_CURRENT_LIST_DIR}/resource.h
)

set(OPEN_RIVAL_ADDITIONAL_DIRECTORIES
    ${CMAKE_CURRENT_LIST_DIR}/libs/freetype-${FREETYPE_VERSION}/include
    ${CMAKE_CURRENT_LIST_DIR}/libs/glew-${GLEW_VERSION}/include
    ${CMAKE_CURRENT_LIST_DIR}/libs/glm-${GLM_VERSION}
    ${CMAKE_CURRENT_LIST_DIR}/libs/json-${JSON_VERSION}/include
    ${CMAKE_CURRENT_LIST_DIR}/libs/openal-soft-${OPENAL_VERSION}/include
    ${CMAKE_CURRENT_LIST_DIR}/libs/rtmidi-${RTMIDI_VERSION}
    ${CMAKE_CURRENT_LIST_DIR}/libs/SDL2-${SDL2_VERSION}/include
    ${CMAKE_CURRENT_LIST_DIR}/libs/SDL2_image-${SDL2_IMAGE_VERSION}/include
)

# Organise files into folders (filters)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "Source Files" FILES ${OPEN_RIVAL_SOURCES})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/include PREFIX "Source Files" FILES ${OPEN_RIVAL_HEADERS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} PREFIX "Resources" FILES ${OPEN_RIVAL_RESOURCES})
source_group("Docs" FILES ${OPEN_RIVAL_DOCS})

# Creates the executable including all the sources and resources
add_executable(Open-Rival
    ${OPEN_RIVAL_SOURCES}
    ${OPEN_RIVAL_HEADERS}
    ${OPEN_RIVAL_RESOURCES}
    ${OPEN_RIVAL_DOCS}
    ${OPEN_RIVAL_EXTRA_FILES}
)

# Add the directories and headers of the project
target_include_directories(Open-Rival PRIVATE
    ${OPEN_RIVAL_INCLUDE_DIRECTORIES}
    ${OPEN_RIVAL_ADDITIONAL_DIRECTORIES}
)

# Adds the directories where the linker can find the libraries
target_link_directories(Open-Rival PUBLIC
    # Debug
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype-${FREETYPE_VERSION}/objs/x64/Debug>
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-${GLEW_VERSION}/lib/Debug/x64>
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft-${OPENAL_VERSION}/build/Debug>
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/libs/rtmidi-${RTMIDI_VERSION}/build/Debug>
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2-${SDL2_VERSION}/lib/x64>
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image-${SDL2_IMAGE_VERSION}/lib/x64>

    # Release
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype-${FREETYPE_VERSION}/objs/x64/Release>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-${GLEW_VERSION}/lib/Release/x64>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft-${OPENAL_VERSION}/build/Release>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/libs/rtmidi-${RTMIDI_VERSION}/build/Release>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2-${SDL2_VERSION}/lib/x64>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image-${SDL2_IMAGE_VERSION}/lib/x64>
)

# Adds the libraries it must link against to find the symbols
target_link_libraries(Open-Rival PRIVATE
    freetype
    $<$<CONFIG:Debug>:glew32sd>
    $<$<CONFIG:Release>:glew32s>
    freetype
    glu32
    OpenAL32
    opengl32
    rtmidi
    SDL2
    SDL2main
    SDL2_image
    winmm
    Ws2_32
    project_options
    project_warnings
)

# Pre-compiled headers
target_precompile_headers(Open-Rival PRIVATE
    <cstdint>
    <iostream>
    <memory>
    <stdexcept>
    <string>
    <vector>
)
