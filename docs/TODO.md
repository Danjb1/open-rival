# To Do

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

 - maxTilesX and maxTilesY should depend on the window size
    - We risk overflowing the buffer in high resolutions

 - The game will crash if a level is smaller than the number of visible tiles

<!----------------------------------------------------------------------------->
## Tech Debt
<!----------------------------------------------------------------------------->

 - Clarify use of unique_ptrs

 - Framebuffer size calculations should use RenderUtils

 - Rendering:
    - Duplication between UnitRenderer and TileRenderer
    - Delete Renderables when a Unit no longer exists
        - If a Unit is erased from memory, our renderable map will have a null key!
    - Why does FramebufferRendering require a different winding order to other renderers?
    - Verify that the correct number of tiles are being rendered

 - ScenarioReader:
    - Use BinaryFileReader
    - Improve error handling
    - Analyse skipped sections; do they vary? Are they read?

 - Destroy Textures / Framebuffer on exit
        glDeleteTextures(1, &id);
        glDeleteFramebuffers(1, &id);

 - Should enums have type specifiers (e.g. TileType is std::int8_t)?

<!----------------------------------------------------------------------------->
## Features
<!----------------------------------------------------------------------------->

### General

 - Set window icon
 - Log to file

### Data Loading

 - Extract menu / interface images
 - Extract fonts
 - Finish parsing Goals
 - Finish parsing Chests (contents)
 - Include missing alphabet entries (e.g. '!{})
 - Respect Gold tile variants

### Game

 - Store unit / building defaults
 - Buildings
 - Mountains
 - Trees
 - Scenery
 - Chests
 - Info Points
 - Doors
 - Game loop
 - Use the Command Pattern to manipulate game state?
 - Use the lockstep model for multiplayer?
 - Moving units to adjacent tiles
 - Pathfinding
 - Building placement
 - Training
 - Resting
 - Attacking
 - Upgrades
 - Experience
 - Food
 - Items
 - Spells
 - Effects (e.g. corpses / explosions)
 - Monster AI
 - Player AI

### Input

 - Panning with the mouse at level edges
 - Smooth scrolling with the arrow keys
 - Camera should zoom towards / away from the mouse position
 - Selecting units (render to a texture with mouse picking shader)
 - Drag-select

### Rendering

 - Shader used for mouse picking
 - Render Interface
 - Render map border
 - Water tile animations
 - Gold tile animations
 - Render Buildings
 - Render Scenery
 - Render Units
    - Animations
    - Respect colour
 - Use depth buffer
 - Fog of war
 - High resolution support (!)

### Audio

 - Play sounds when selecting units
 - Play sounds when instructing units to move
 - Add support for WMA music
 - Add support for MIDI music

### Menus

 - Main menu
 - Loading screen
 - Custom Map menu
 - Pause menu
 - Troop libraries

### Editor

 - Create a ScenarioWriter class

### Future Enhancements

 - [ ] Easy military drag-select
 - [ ] Rally points
 - [ ] Configurable Gold multiplier
 - [ ] Multiplayer campaigns
 - [ ] Stable net play!
 - [ ] Random map generation
 - [ ] More endgame stats
 - [ ] Improved pathfinding
 - [ ] Non-suicidal peasants (don't go looking for gold)
 - [ ] Affordable Wall
 - [ ] Map preview / description
 - [ ] Using abilities with a group selected
 - [ ] Rank explanations
 - [ ] Replays
 - [ ] Multilanguage support
 - [ ] ARM version (for Raspberry Pi)
 - [ ] Windows XP support
 - [ ] Support for larger screen resolutions
 - [ ] Support for bigger maps
 - [x] Camera zooming

<!----------------------------------------------------------------------------->
## Project
<!----------------------------------------------------------------------------->

 - Static code analysis
 - Unit tests
