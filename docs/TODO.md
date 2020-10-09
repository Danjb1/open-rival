# To Do

<!----------------------------------------------------------------------------->
## WIP
<!----------------------------------------------------------------------------->

 - Store Components in a map

 - Check all Animation data; some values are incorrect

 - When a Unit is deleted, any references to it will become invalid
    - We need a way to check if a Unit reference is still valid!
    - Just store entity IDs instead of references, and retrieve the Entity each frame

 - Render the UI

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

 - extractImages() method in setup/src/image-extractor.cpp assumes at most 63
character filename (including path)

<!----------------------------------------------------------------------------->
## Tech Debt
<!----------------------------------------------------------------------------->

 - Image filenames can be truncated due to limited size of char array

 - Framebuffer size calculations should use RenderUtils

 - Why does FramebufferRendering require a different winding order to other renderers?

 - ScenarioReader:
    - Use BinaryFileReader
    - Improve error handling
    - Analyse skipped sections; do they vary? Are they read?
    - Consider rewriting ScenarioReader and ScenarioBuilder as free-standing functions, e.g.
        auto scenario_desc = readScenario(scenarioPath);
        auto scenario = buildScenario(scenario_desc);
        - These could live inside a common namespace (e.g. ScenarioUtils)

 - Should enums have type specifiers (e.g. TileType is std::uint8_t)?

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

 - Store unit / building defaults
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
 - Panning with middle-mouse button (drag)
 - Smooth scrolling with the arrow keys
 - Scroll speed should depend on the zoom level
 - Drag-select

### Rendering

 - Render Interface
 - Water tile animations
 - Gold tile animations
 - Render Scenery
 - Respect unit colour
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

### Quality-of-Life Improvements

 - [x] 60 FPS
 - [x] Camera zooming
 - [ ] Improved camera navigation
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
 - [ ] Support for larger screen resolutions
 - [ ] Support for bigger maps
 - [ ] Upscale graphics using AI?

<!----------------------------------------------------------------------------->
## Project
<!----------------------------------------------------------------------------->

 - Remove assets from git history
 - Build for Linux
 - Add script to build from command line
 - Add script to run tests from command line
 
<!----------------------------------------------------------------------------->
## Unit Tests
<!----------------------------------------------------------------------------->

 - Mock Shaders / ShaderUtils in tests instead of mocking so many library functions?
