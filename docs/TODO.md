# To Do

<!----------------------------------------------------------------------------->
## Next Up
<!----------------------------------------------------------------------------->

 - Pass Resources to Application constructor
    - Use fake Resources in tests

 - Animation support
    - Check all Animation data; some values are incorrect
    - Flying units should animate on the spot
    - Check move / attack animations
    - Death animations require special facing logic!

 - When a Unit is deleted, any references to it will become invalid
    - We need a way to check if a Unit reference is still valid!
    - Just store entity IDs instead of references, and retrieve the Entity each frame
    - We can't enforce this, but it should at least be documented!

 - Render the UI
    - Use (and document) a sensible co-ordinate system (currently normalized device coordinates: -1 to 1)
    - Leave space for minimap and stats panel!
    - Currently the UI is too tall and overlaps with the game
    - AtlasRenderable and SpriteRenderable make references to specific shaders
    - Extract common code from SpriteRenderable and AtlasRenderable
    - Use string constants to access images within a texture atlas
    - Render the UI directly below the viewport
    - Render Greenskin UI, dependent on race: img_ui_01A07D3D,tga
    - Render inventory overlay at (30, 71)
         - Human: img_ui_019F3D4E.tga
         - Greenskin: img_ui_01A15E63.tga
         - Ideally, pack UI images into a single Renderable

 - Palette
    - Separate team colours into their own arrays
    - Build a palette for each team colour dynamically
    - Render units using their correct colours (unit colours, not team colours!)

 - Gold tile animations
    - Respect Gold tile variants
    - Each frame, call `Scenario.update`
    - This should pick 20 tiles randomly from those that are visible
    - Any gold tiles that get picked should be animated:
        - Keep a list of goldTileAnimations
        - Add an entry to this list
        - Re-use logic from AnimationComponent
    - Add an extra step to the TileRenderer to render these animations on top
        of the relevant gold tiles

 - Water tile animations
    - These animations have not been extracted!
    - Check if animations can be produced by palette swapping
    - Check if animations can be produced by supplying extra params during image extraction
    - Water tiles all animate at the same time, so can be handled globally
    - Have the tile renderer look up the current animation frame when drawing water tiles

 - Logging mechanism
    - Log important events to file

 - Multiplayer
    - Server code should be separated somehow
        - 3 projects: client, server and common
        - Client depends on the server, since the client can act as a host
        - Server can be run as a standalone application
    - Clients should connect to server using TCP
    - Use a lock-step model, but with a client-server architecture
        - See [multiplayer notes](/docs/multiplayer.md)
        - https://www.informit.com/articles/article.aspx?p=2461064&seqNum=3
        - https://gafferongames.com/post/what_every_programmer_needs_to_know_about_game_networking/
    - Server awaits commands from all clients and executes them at fixed intervals, or "turns"
    - Server sends the commands from each turn to all clients
    - Each client runs a deterministic simulation of the game by applying these commands

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

 - extractImages() method in setup/src/image-extractor.cpp assumes at most 63
character filename (including path)

<!----------------------------------------------------------------------------->
## Tech Debt
<!----------------------------------------------------------------------------->

 - runtime_error is *sometimes* initialised with a newline-terminated string

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

 - Move constants from header files to CPP files

<!----------------------------------------------------------------------------->
## Features
<!----------------------------------------------------------------------------->

### General

 - Set window icon
 - Custom cursors

### Data Loading

 - Pack interface images into textures
 - Add interface extraction to setup application
 - Extract fonts
 - Finish parsing Goals
 - Finish parsing Chests (contents)
 - Include missing alphabet entries (e.g. '!{})

### Game

 - Store unit / building defaults
 - Mountains
 - Trees
 - Scenery
 - Chests
 - Info Points
 - Doors
 - Use the Command Pattern to manipulate game state?
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

 - Render interface
 - Render trees
 - Render mountains
 - Render info points
 - Render chests
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
 - [ ] UI scaling
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
