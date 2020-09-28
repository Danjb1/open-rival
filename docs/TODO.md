# To Do

<!----------------------------------------------------------------------------->
## WIP
<!----------------------------------------------------------------------------->

 - Refactor animations
    - Remove tuples: https://stackoverflow.com/questions/44650636/when-to-use-tuple-vs-class-c-sharp-7-0
    - Use a component-based system to keep animations and units loosely coupled
    - Instead of having the UnitRenderer track the presence of all units, have the units register / unregister themselves against the renderer
        - Use an interface (e.g. GraphicsContext) so we are not tied to a specific implementation

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

 - Resources refactor
    - Pass Resources around instead of individual textures / spritesheets?

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
 - Add a license
 - Static code analysis
 - Unit tests
