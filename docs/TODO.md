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

<!----------------------------------------------------------------------------->
## Features
<!----------------------------------------------------------------------------->

### General

 - Set window icon
 - Log to file
 - Static code analysis
 - Unit tests

### Editor

 - Create a ScenarioWriter class

### Image Preparation

 - Extract menu / interface images
 - Extract fonts

### Scenario Loading

 - Finish parsing Goals
 - Finish parsing Chests (contents)
 - Include missing alphabet entries (e.g. '!{})
 - Respect Gold tile variants

### Engine

 - Create data structures:
    - Map
    - TileType
    - UnitType
    - BuildingType
    - ItemType
    - GameObject
        - Unit (Ground / Flying)
        - Building
        - Obstacle
        - Door
        - Effect
 - Scene organisation / traversal
    - A single tile might have:
        - Terrain
        - Cropland
        - A ground unit
        - A flying unit
        - An effect (such as a corpse animation)
    - Ability to list all units
    - Ability to get a unit by ID
    - Ability to know which tile a given unit occupies
    - Ability to know which GameObject and flying unit occupies a tile
    - Ability to know the type of a tile
    - Ability to deduce the passability of a tile
    - Ability to list all effects
 - Game loop

### Game Logic

 - Use the Command Pattern to manipulate game state?
 - Use the lockstep model for multiplayer?
 - Moving units to adjacent tiles
 - Pathfinding
 - Building
 - Training
 - Resting
 - Attacking
 - Upgrades
 - Experience
 - Food
 - Items
 - Spells
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
 - Add support for animated tiles
 - Render Buildings
 - Render Obstacles
 - Render Units
    - Different animations
    - Add support for team colours
 - Use depth buffer
 - Fog of war
 - High resolution support (!)

### Sound

 - Play sounds when selecting units
 - Play sounds when instructing units to move

### Music

 - WMA
 - MIDI

### Menus

 - Main menu
 - Loading screen
 - Custom map menu
 - Pause menu

### File Formats

 - Find campaign map data

### Future Enhancements

 - Easy military drag-select
 - Rally points
 - Configurable Gold multiplier
 - Multiplayer campaigns
 - Stable net play!
 - Random map generation
 - More endgame stats
 - Improved pathfinding
 - Non-suicidal peasants (don't go looking for gold)
 - Affordable Wall
 - Map preview / description
 - Using abilities with a group selected
 - Rank explanations
 - Replays
 - Multilanguage support
 - ARM version (for Raspberry Pi)
 - Windows XP support
 - Support for larger screen resolutions
 - Support for bigger maps

<!----------------------------------------------------------------------------->
## Project
<!----------------------------------------------------------------------------->

 - Static code analysis
 - Unit tests
