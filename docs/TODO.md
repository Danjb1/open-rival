# To Do

<!----------------------------------------------------------------------------->
## WIP
<!----------------------------------------------------------------------------->

 - Entity / Component system
    - Encapsulate unit creation in a UnitFactory
    - Encapsulate building creation in a BuildingFactory
    - Buildings need a SpriteComponent
    - UnitType should be a component
    - BuildingType should be a component
    - Facing should be a component
    - Player should be a component
    - WallVariant should be a component
    - SpriteComponent should figure out the correct txIndex from the facing and animation!
    - Set passability correctly when adding an Entitiy to a Scenario
    - MousePicker should determine hitbox offsets based on entity type
    - Fix unit tests!

 - Refactor animations animations
    - Reduce duplication between UnitAnimation and BuildingAnimation
    - Remove tuples: https://stackoverflow.com/questions/44650636/when-to-use-tuple-vs-class-c-sharp-7-0

 - When a Unit is deleted, any references to it will become invalid
    - We need a way to check if a Unit reference is still valid!

 - Render the UI

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

 - extractImages() method in setup/src/image-extractor.cpp assumes at most 63
character filename (including path)

<!----------------------------------------------------------------------------->
## Tech Debt
<!----------------------------------------------------------------------------->

 - Reduce duplication between MousePicker and zooming logic

 - Image filenames can be truncated due to limited size of char array

 - Framebuffer size calculations should use RenderUtils

 - Mouse picking:
    - We could use [this strategy](https://www.kamremake.com/devblog/unit-picking/),
        and use the render result from the previous frame!
    - This would be much simpler, and possibly faster, BUT would involve an extra
        rendering step
    - The current solution can be optimised by only considering on-screen units

 - Rendering:
    - Why does FramebufferRendering require a different winding order to other renderers?

 - ScenarioReader:
    - Use BinaryFileReader
    - Improve error handling
    - Analyse skipped sections; do they vary? Are they read?

 - Should enums have type specifiers (e.g. TileType is std::uint8_t)?
 
 - Consider rewriting ScenarioReader and ScenarioBuilder as free-standing functions, e.g.
        auto scenario_desc = readScenario(scenarioPath);
        auto scenario = buildScenario(scenario_desc);
   These could live inside a common namespace (e.g. ScenarioUtils)

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
 - Panning with middle-mouse button (drag)
 - Smooth scrolling with the arrow keys
 - Scroll speed should depend on the zoom level
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
