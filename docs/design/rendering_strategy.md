# Rendering Strategy

## Aims

 - Game models (e.g. `Unit`) should not contain any information specific to the rendering implementation
 - It should be possible to change the rendering technique just by swapping out the renderer (e.g. to enable software rendering or bypass rendering entirely)
 - Shader and texture switching should be minimised for performance
 - OpenGL code should be clearly readable with minimal abstraction

## VAOs

 - VAOs should be stored in the relevant Renderer classes
 - Everything contained within a VAO needs to share the same texture and shader
 - Buffers should be sent to the GPU each frame based on what is visible to the camera

### Tiles

 - 1 VAO for all Tiles

### Scenery

 - 1 VAO for all "common" objects (info points, rocks, chests, bags)
 - 1 VAO all tilset-specific objects (trees, mountains, dungeon wall, doors)

### Units

 - 1 VAO per Unit
 - When a unit is deleted, the corresponding VAO must be deleted too

### Buildings

 - 1 VAO per team

## Animations

 - Animation data can be part of the game model
 - Renderers can determine the texture co-ordinates based on the sprite and current animation frame

## Depth

 - Depth buffer should be enabled to ensure correct z-ordering
 - z co-ordinate should be determined based on layer (tile / entity) and x/y position
