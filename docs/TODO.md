# To Do

<!----------------------------------------------------------------------------->
## WIP
<!----------------------------------------------------------------------------->

 - Mouse picking units

 - Resources refactor
    - Follow rule of 5
    - Pass Resources around instead of individual textures / spritesheets?

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

 - maxTilesX and maxTilesY should depend on the window size
    - We risk overflowing the buffer in high resolutions

 - The game will crash if a level is smaller than the number of visible tiles

<!----------------------------------------------------------------------------->
## Tech Debt
<!----------------------------------------------------------------------------->

 - Camera should point at the centre of a tile - not the top-left corner

 - Clarify use of unique_ptrs

 - Framebuffer size calculations should use RenderUtils

 - Viewport should be defined explicitly rather than relying on windowWidth/Height

 - Rendering:
    - Duplication between Renderers
    - Delete Renderables when a Unit no longer exists
        - If a Unit is erased from memory, our renderable map will have a null key!
    - Why does FramebufferRendering require a different winding order to other renderers?

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

 - Game logic should run at a consistent rate, regardless of monitor refresh rate
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
 - Drag-select

### Rendering

 - Render Interface
 - Water tile animations
 - Gold tile animations
 - Render Buildings
 - Render Scenery
 - Render Units
    - Animations
    - Respect colour
    - Sort by z-order!
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
