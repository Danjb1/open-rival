# Rendering Strategy

## Aims

- Game logic and rendering should be completely separate.
    - No game logic should include any OpenGL dependencies.
    - Rendering logic should be completely separate from any game state.
    - It should be possible to swap out the renderer (e.g. headless, OpenGL, Vulkan) from a single point in the code.
- Game logic includes the spritesheet and animation data, but no VAOs.

## OpenGL Strategy

- Shader and texture switching should be minimised for performance.
- OpenGL code should be clearly readable with minimal abstraction.
- Everything contained within a VAO needs to share the same texture and shader.
- Buffers should be sent to the GPU each frame based on what is visible to the camera.
- VAOs should be owned by "Renderables".
- Renderables should be owned by "Renderers".
- Depth buffer should be enabled to ensure correct z-ordering.
    - z co-ordinate should be determined based on layer (tile / entity) and x/y position.

### VAOs

Currently we use:

- 1 VAO to render all Tiles.
- 1 VAO per Entity.

Later, we could optimise this, e.g. by grouping together similar objects:

- 1 VAO shared by "common" objects (info points, rocks, chests, bags).
- 1 VAO shared by all tileset-specific objects (trees, mountains, dungeon wall, doors).
- 1 VAO shared by all buildings of each race.

Essentially, anything that shares a texture can share a VAO.
