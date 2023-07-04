set(OPEN_RIVAL_SOURCES
    ${OPEN_RIVAL_DIR}/src/application/Application.cpp
    ${OPEN_RIVAL_DIR}/src/application/ApplicationContext.cpp
    ${OPEN_RIVAL_DIR}/src/application/Resources.cpp
    ${OPEN_RIVAL_DIR}/src/application/State.cpp
    ${OPEN_RIVAL_DIR}/src/application/Window.cpp
    ${OPEN_RIVAL_DIR}/src/audio/AudioSystem.cpp
    ${OPEN_RIVAL_DIR}/src/audio/AudioUtils.cpp
    ${OPEN_RIVAL_DIR}/src/audio/MidiContainer.cpp
    ${OPEN_RIVAL_DIR}/src/audio/MidiFile.cpp
    ${OPEN_RIVAL_DIR}/src/audio/MidiPlayer.cpp
    ${OPEN_RIVAL_DIR}/src/audio/MidsDecoder.cpp
    ${OPEN_RIVAL_DIR}/src/audio/SoundSource.cpp
    ${OPEN_RIVAL_DIR}/src/audio/Sounds.cpp
    ${OPEN_RIVAL_DIR}/src/audio/WaveFile.cpp
    ${OPEN_RIVAL_DIR}/src/commands/GameCommand.cpp
    ${OPEN_RIVAL_DIR}/src/commands/GameCommandFactory.cpp
    ${OPEN_RIVAL_DIR}/src/commands/MoveCommand.cpp
    ${OPEN_RIVAL_DIR}/src/entity/Entity.cpp
    ${OPEN_RIVAL_DIR}/src/entity/EntityFactory.cpp
    ${OPEN_RIVAL_DIR}/src/entity/EntityRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/BuildingAnimationComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/BuildingPropsComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/EntityComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/FacingComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/FlyerComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/MouseHandlerComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/MovementComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/OwnerComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/PassabilityComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/PortraitComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/SeafarerComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/SpriteComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/UnitAnimationComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/UnitPropsComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/VoiceComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/WalkerComponent.cpp
    ${OPEN_RIVAL_DIR}/src/entity/components/WallComponent.cpp
    ${OPEN_RIVAL_DIR}/src/game/Animations.cpp
    ${OPEN_RIVAL_DIR}/src/game/Building.cpp
    ${OPEN_RIVAL_DIR}/src/game/BuildingDef.cpp
    ${OPEN_RIVAL_DIR}/src/game/GameInterface.cpp
    ${OPEN_RIVAL_DIR}/src/game/GameState.cpp
    ${OPEN_RIVAL_DIR}/src/game/InventoryComponent.cpp
    ${OPEN_RIVAL_DIR}/src/game/MapUtils.cpp
    ${OPEN_RIVAL_DIR}/src/game/MousePicker.cpp
    ${OPEN_RIVAL_DIR}/src/game/PathUtils.cpp
    ${OPEN_RIVAL_DIR}/src/game/Pathfinding.cpp
    ${OPEN_RIVAL_DIR}/src/game/PlayerContext.cpp
    ${OPEN_RIVAL_DIR}/src/game/PlayerState.cpp
    ${OPEN_RIVAL_DIR}/src/game/Tile.cpp
    ${OPEN_RIVAL_DIR}/src/game/UnitDef.cpp
    ${OPEN_RIVAL_DIR}/src/game/World.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Camera.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Color.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Font.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Framebuffer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/GLUtils.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Image.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Palette.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/PaletteUtils.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/RenderUtils.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/ShaderUtils.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Shaders.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Spritesheet.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/Texture.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/TextureAtlas.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderable/AtlasRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderable/BoxRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderable/SpriteRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderable/TextRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderable/gfx/TextureRenderable.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/FramebufferRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/GameRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/MapBorderRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/MenuTextRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/TextRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/TileRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/gfx/renderer/UiRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/lobby/LobbyState.cpp
    ${OPEN_RIVAL_DIR}/src/main/Main.cpp
    ${OPEN_RIVAL_DIR}/src/main/ProgramOptions.cpp
    ${OPEN_RIVAL_DIR}/src/net/ClientInfo.cpp
    ${OPEN_RIVAL_DIR}/src/net/Connection.cpp
    ${OPEN_RIVAL_DIR}/src/net/PacketFactory.cpp
    ${OPEN_RIVAL_DIR}/src/net/Server.cpp
    ${OPEN_RIVAL_DIR}/src/net/Socket.cpp
    ${OPEN_RIVAL_DIR}/src/net/WindowsNetUtils.cpp
    ${OPEN_RIVAL_DIR}/src/net/packet-handlers/AcceptPlayerPacketHandler.cpp
    ${OPEN_RIVAL_DIR}/src/net/packet-handlers/GameCommandPacketHandler.cpp
    ${OPEN_RIVAL_DIR}/src/net/packet-handlers/KickPlayerPacketHandler.cpp
    ${OPEN_RIVAL_DIR}/src/net/packet-handlers/LobbyWelcomePacketHandler.cpp
    ${OPEN_RIVAL_DIR}/src/net/packet-handlers/PacketHandler.cpp
    ${OPEN_RIVAL_DIR}/src/net/packet-handlers/RejectPlayerPacketHandler.cpp
    ${OPEN_RIVAL_DIR}/src/net/packet-handlers/RequestJoinPacketHandler.cpp
    ${OPEN_RIVAL_DIR}/src/net/packet-handlers/StartGamePacketHandler.cpp
    ${OPEN_RIVAL_DIR}/src/net/packets/AcceptPlayerPacket.cpp
    ${OPEN_RIVAL_DIR}/src/net/packets/GameCommandPacket.cpp
    ${OPEN_RIVAL_DIR}/src/net/packets/KickPlayerPacket.cpp
    ${OPEN_RIVAL_DIR}/src/net/packets/LobbyWelcomePacket.cpp
    ${OPEN_RIVAL_DIR}/src/net/packets/Packet.cpp
    ${OPEN_RIVAL_DIR}/src/net/packets/RejectPlayerPacket.cpp
    ${OPEN_RIVAL_DIR}/src/net/packets/RelayedPacket.cpp
    ${OPEN_RIVAL_DIR}/src/net/packets/RequestJoinPacket.cpp
    ${OPEN_RIVAL_DIR}/src/net/packets/StartGamePacket.cpp
    ${OPEN_RIVAL_DIR}/src/platform/unix/UnixTimeUtils.cpp
    ${OPEN_RIVAL_DIR}/src/platform/win32/WindowsSocket.cpp
    ${OPEN_RIVAL_DIR}/src/platform/win32/WindowsTimeUtils.cpp
    ${OPEN_RIVAL_DIR}/src/scenario/ScenarioBuilder.cpp
    ${OPEN_RIVAL_DIR}/src/scenario/ScenarioReader.cpp
    ${OPEN_RIVAL_DIR}/src/ui/Cursor.cpp
    ${OPEN_RIVAL_DIR}/src/ui/CursorRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/ui/MenuRenderer.cpp
    ${OPEN_RIVAL_DIR}/src/utils/BinaryFileReader.cpp
    ${OPEN_RIVAL_DIR}/src/utils/BufferUtils.cpp
    ${OPEN_RIVAL_DIR}/src/utils/FileUtils.cpp
    ${OPEN_RIVAL_DIR}/src/utils/InputUtils.cpp
    ${OPEN_RIVAL_DIR}/src/utils/JsonUtils.cpp
    ${OPEN_RIVAL_DIR}/src/utils/MathUtils.cpp
    ${OPEN_RIVAL_DIR}/src/utils/MouseUtils.cpp
    ${OPEN_RIVAL_DIR}/src/utils/Rect.cpp
    ${OPEN_RIVAL_DIR}/src/utils/StringUtils.cpp
    ${OPEN_RIVAL_DIR}/src/utils/TimeUtils.cpp
)

set(OPEN_RIVAL_HEADERS
    ${OPEN_RIVAL_DIR}/include/ConfigUtils.h
    ${OPEN_RIVAL_DIR}/include/EntityUtils.h
    ${OPEN_RIVAL_DIR}/include/EnumUtils.h
    ${OPEN_RIVAL_DIR}/include/Race.h
    ${OPEN_RIVAL_DIR}/include/SDLWrapper.h
    ${OPEN_RIVAL_DIR}/include/ScenarioData.h
    ${OPEN_RIVAL_DIR}/include/Unit.h
    ${OPEN_RIVAL_DIR}/include/application/Application.h
    ${OPEN_RIVAL_DIR}/include/application/ApplicationContext.h
    ${OPEN_RIVAL_DIR}/include/application/Resources.h
    ${OPEN_RIVAL_DIR}/include/application/State.h
    ${OPEN_RIVAL_DIR}/include/application/Window.h
    ${OPEN_RIVAL_DIR}/include/audio/AudioSystem.h
    ${OPEN_RIVAL_DIR}/include/audio/AudioUtils.h
    ${OPEN_RIVAL_DIR}/include/audio/MidiContainer.h
    ${OPEN_RIVAL_DIR}/include/audio/MidiFile.h
    ${OPEN_RIVAL_DIR}/include/audio/MidiPlayer.h
    ${OPEN_RIVAL_DIR}/include/audio/MidsDecoder.h
    ${OPEN_RIVAL_DIR}/include/audio/SoundSource.h
    ${OPEN_RIVAL_DIR}/include/audio/Sounds.h
    ${OPEN_RIVAL_DIR}/include/audio/WaveFile.h
    ${OPEN_RIVAL_DIR}/include/commands/GameCommand.h
    ${OPEN_RIVAL_DIR}/include/commands/GameCommandFactory.h
    ${OPEN_RIVAL_DIR}/include/commands/MoveCommand.h
    ${OPEN_RIVAL_DIR}/include/entity/Entity.h
    ${OPEN_RIVAL_DIR}/include/entity/EntityFactory.h
    ${OPEN_RIVAL_DIR}/include/entity/EntityRenderer.h
    ${OPEN_RIVAL_DIR}/include/entity/components/BuildingAnimationComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/BuildingPropsComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/EntityComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/FacingComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/FlyerComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/MouseHandlerComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/MovementComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/OwnerComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/PassabilityComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/PortraitComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/SeafarerComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/SpriteComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/UnitAnimationComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/UnitPropsComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/VoiceComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/WalkerComponent.h
    ${OPEN_RIVAL_DIR}/include/entity/components/WallComponent.h
    ${OPEN_RIVAL_DIR}/include/game/Animations.h
    ${OPEN_RIVAL_DIR}/include/game/Building.h
    ${OPEN_RIVAL_DIR}/include/game/BuildingDef.h
    ${OPEN_RIVAL_DIR}/include/game/GameInterface.h
    ${OPEN_RIVAL_DIR}/include/game/GameState.h
    ${OPEN_RIVAL_DIR}/include/game/InventoryComponent.h
    ${OPEN_RIVAL_DIR}/include/game/MapUtils.h
    ${OPEN_RIVAL_DIR}/include/game/MousePicker.h
    ${OPEN_RIVAL_DIR}/include/game/PathUtils.h
    ${OPEN_RIVAL_DIR}/include/game/Pathfinding.h
    ${OPEN_RIVAL_DIR}/include/game/PlayerContext.h
    ${OPEN_RIVAL_DIR}/include/game/PlayerState.h
    ${OPEN_RIVAL_DIR}/include/game/Tile.h
    ${OPEN_RIVAL_DIR}/include/game/UnitDef.h
    ${OPEN_RIVAL_DIR}/include/game/World.h
    ${OPEN_RIVAL_DIR}/include/gfx/Camera.h
    ${OPEN_RIVAL_DIR}/include/gfx/Color.h
    ${OPEN_RIVAL_DIR}/include/gfx/Font.h
    ${OPEN_RIVAL_DIR}/include/gfx/Framebuffer.h
    ${OPEN_RIVAL_DIR}/include/gfx/GLUtils.h
    ${OPEN_RIVAL_DIR}/include/gfx/GlewWrapper.h
    ${OPEN_RIVAL_DIR}/include/gfx/Image.h
    ${OPEN_RIVAL_DIR}/include/gfx/Palette.h
    ${OPEN_RIVAL_DIR}/include/gfx/PaletteUtils.h
    ${OPEN_RIVAL_DIR}/include/gfx/RenderUtils.h
    ${OPEN_RIVAL_DIR}/include/gfx/ShaderUtils.h
    ${OPEN_RIVAL_DIR}/include/gfx/Shaders.h
    ${OPEN_RIVAL_DIR}/include/gfx/Spritesheet.h
    ${OPEN_RIVAL_DIR}/include/gfx/Texture.h
    ${OPEN_RIVAL_DIR}/include/gfx/TextureAtlas.h
    ${OPEN_RIVAL_DIR}/include/gfx/renderable/AtlasRenderable.h
    ${OPEN_RIVAL_DIR}/include/gfx/renderable/BoxRenderable.h
    ${OPEN_RIVAL_DIR}/include/gfx/renderable/SpriteRenderable.h
    ${OPEN_RIVAL_DIR}/include/gfx/renderable/TextRenderable.h
    ${OPEN_RIVAL_DIR}/include/gfx/renderable/gfx/TextureRenderable.h
    ${OPEN_RIVAL_DIR}/include/gfx/renderer/FramebufferRenderer.h
    ${OPEN_RIVAL_DIR}/include/gfx/renderer/GameRenderer.h
    ${OPEN_RIVAL_DIR}/include/gfx/renderer/MapBorderRenderer.h
    ${OPEN_RIVAL_DIR}/include/gfx/renderer/MenuTextRenderer.h
    ${OPEN_RIVAL_DIR}/include/gfx/renderer/TextRenderer.h
    ${OPEN_RIVAL_DIR}/include/gfx/renderer/TileRenderer.h
    ${OPEN_RIVAL_DIR}/include/gfx/renderer/UiRenderer.h
    ${OPEN_RIVAL_DIR}/include/lobby/LobbyState.h
    ${OPEN_RIVAL_DIR}/include/main/ProgramOptions.h
    ${OPEN_RIVAL_DIR}/include/net/ClientInfo.h
    ${OPEN_RIVAL_DIR}/include/net/Connection.h
    ${OPEN_RIVAL_DIR}/include/net/NetUtils.h
    ${OPEN_RIVAL_DIR}/include/net/PacketFactory.h
    ${OPEN_RIVAL_DIR}/include/net/Server.h
    ${OPEN_RIVAL_DIR}/include/net/Socket.h
    ${OPEN_RIVAL_DIR}/include/net/packet-handlers/AcceptPlayerPacketHandler.h
    ${OPEN_RIVAL_DIR}/include/net/packet-handlers/GameCommandPacketHandler.h
    ${OPEN_RIVAL_DIR}/include/net/packet-handlers/KickPlayerPacketHandler.h
    ${OPEN_RIVAL_DIR}/include/net/packet-handlers/LobbyWelcomePacketHandler.h
    ${OPEN_RIVAL_DIR}/include/net/packet-handlers/PacketHandler.h
    ${OPEN_RIVAL_DIR}/include/net/packet-handlers/RejectPlayerPacketHandler.h
    ${OPEN_RIVAL_DIR}/include/net/packet-handlers/RequestJoinPacketHandler.h
    ${OPEN_RIVAL_DIR}/include/net/packet-handlers/StartGamePacketHandler.h
    ${OPEN_RIVAL_DIR}/include/net/packets/AcceptPlayerPacket.h
    ${OPEN_RIVAL_DIR}/include/net/packets/GameCommandPacket.h
    ${OPEN_RIVAL_DIR}/include/net/packets/KickPlayerPacket.h
    ${OPEN_RIVAL_DIR}/include/net/packets/LobbyWelcomePacket.h
    ${OPEN_RIVAL_DIR}/include/net/packets/Packet.h
    ${OPEN_RIVAL_DIR}/include/net/packets/RejectPlayerPacket.h
    ${OPEN_RIVAL_DIR}/include/net/packets/RelayedPacket.h
    ${OPEN_RIVAL_DIR}/include/net/packets/RequestJoinPacket.h
    ${OPEN_RIVAL_DIR}/include/net/packets/StartGamePacket.h
    ${OPEN_RIVAL_DIR}/include/scenario/ScenarioBuilder.h
    ${OPEN_RIVAL_DIR}/include/scenario/ScenarioReader.h
    ${OPEN_RIVAL_DIR}/include/ui/Cursor.h
    ${OPEN_RIVAL_DIR}/include/ui/CursorRenderer.h
    ${OPEN_RIVAL_DIR}/include/ui/MenuRenderer.h
    ${OPEN_RIVAL_DIR}/include/utils/BinaryFileReader.h
    ${OPEN_RIVAL_DIR}/include/utils/BufferUtils.h
    ${OPEN_RIVAL_DIR}/include/utils/FileUtils.h
    ${OPEN_RIVAL_DIR}/include/utils/InputUtils.h
    ${OPEN_RIVAL_DIR}/include/utils/JsonUtils.h
    ${OPEN_RIVAL_DIR}/include/utils/MathUtils.h
    ${OPEN_RIVAL_DIR}/include/utils/MouseUtils.h
    ${OPEN_RIVAL_DIR}/include/utils/Rect.h
    ${OPEN_RIVAL_DIR}/include/utils/StringUtils.h
    ${OPEN_RIVAL_DIR}/include/utils/TimeUtils.h
)