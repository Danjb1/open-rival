# To Do

<!----------------------------------------------------------------------------->
## WIP: Interface
<!----------------------------------------------------------------------------->

 - Use the palette of the first source image in texture-builder

 - Refactor texture-builder

 - Port texture-builder changes to setup

 - Add Viewport class rather than relying on windowWidth/Height

 - Shrink the viewport to create space for the UI

 - Render the UI

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

 - maxTilesX and maxTilesY should depend on the window size
    - We risk overflowing the buffer in high resolutions

 - The game will crash if a level is smaller than the number of visible tiles

 - When zoomed enough, we can see blue dotted lines, probably an artefact from
rendering to offscreen buffer

 - extractImages() method in setup/src/image-extractor.cpp assumes at most 63
character filename (including path)

<!----------------------------------------------------------------------------->
## Tech Debt
<!----------------------------------------------------------------------------->

 - Auto-format code (VS Code Cleanup? CodeMaid? Clang?)

 - Resources refactor
    - Pass Resources around instead of individual textures / spritesheets?

 - Remove unnecessary unique_ptrs

 - Image filenames can be truncated due to limited size of char array

 - Camera should point at the centre of a tile - not the top-left corner

 - Framebuffer size calculations should use RenderUtils

 - Mouse picking:
    - We could use [this strategy](https://www.kamremake.com/devblog/unit-picking/),
        and use the render result from the previous frame!
    - This would be much simpler, and possibly faster, BUT would involve an extra
        rendering step
    - The current solution can be optimised by only considering on-screen units

 - Rendering:
    - Duplication between Renderers
    - Delete Renderables when a Unit no longer exists
        - If a Unit is erased from memory, our renderable map will have a null key!
    - Delete Renderables when a Building no longer exists
        - If a Building is erased from memory, our renderable map will have a null key!
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
 - Custom cursors
 - Log to file

### Data Loading

 - Pack interface images into textures
 - Add interface extraction to setup application
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
 - Use appropriate z-positions for entities
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

 - Remove assets from git history
 - Add a license
 - Static code analysis
 - Unit tests
