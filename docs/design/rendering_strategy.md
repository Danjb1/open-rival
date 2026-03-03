# Rendering Strategy

## Overall Aims

- Game logic and rendering should be completely separate.
    - No game logic should include any OpenGL dependencies.
    - Rendering logic should be completely separate from any game state.
    - It should be possible to swap out the renderer (e.g. headless, OpenGL, Vulkan) from a single point in the code.
- Animation data can be part of the game model.
    - Renderers can determine the texture co-ordinates based on the sprite and current animation frame.

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

- 1 VAO shared by all Tiles.
- 1 VAO shared by "common" objects (info points, rocks, chests, bags).
- 1 VAO shared by all tileset-specific objects (trees, mountains, dungeon wall, doors).
- 1 VAO per Unit.
    - When a unit is deleted, the corresponding VAO must be deleted too!
- 1 VAO per team.
