# To Do

<!----------------------------------------------------------------------------->
## WIP
<!----------------------------------------------------------------------------->

 - N/A

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

 - Do iterators break if an Entity is added from inside the loop?
 - Game will crash if no config file exists
 - extractImages() method in setup/src/image-extractor.cpp assumes at most 63 character filename (including path)
 - Image filenames can be truncated due to limited size of char array

<!----------------------------------------------------------------------------->
## Features
<!----------------------------------------------------------------------------->

### General

 - Set window icon
 - Custom cursors
 - Logging mechanism
    - Log important events to file
    - Replace cout / cerr with logger calls

### Data Loading

 - Pack interface images into textures
 - Add interface extraction to setup application
 - Finish parsing Goals
 - Finish parsing Chests (contents)
 - Include missing alphabet entries (e.g. '!{})

### Gameplay

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

### Multiplayer

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

### Input

 - Panning with the mouse at level edges
 - Panning with middle-mouse button (drag)
 - Smooth scrolling with the arrow keys
 - Scroll speed should depend on the zoom level
 - Drag-select

### Rendering

 - Render units "between" logical frames based on delta values
 - Load / render fonts using FreeType
 - Render interface
    - Render resource icons
 - Render minimap
 - Render info points
 - Render chests
 - Respect unit colour
 - Respect Gold tile variants
 - Fog of war

### Animations

 - Check all Animation data; some values are incorrect
 - Flying units should animate on the spot
 - Check move / attack animations
 - Death animations require special facing logic!
 - Trees should not all animate in-sync
 - Gold tile animations
    - Each frame, call `Scenario.update`
    - This should pick 20 tiles randomly from those that are visible
    - Any gold tiles that get picked should be animated:
        - Keep a list of goldTileAnimations
        - Add an entry to this list
        - Re-use logic from AnimationComponent
    - Add an extra step to the TileRenderer to render these animations on top of the relevant gold tiles
 - Water tile animations
    - These animations have not been extracted!
    - Check if animations can be produced by palette swapping
    - Check if animations can be produced by supplying extra params during image extraction
    - Water tiles all animate at the same time, so can be handled globally
    - Have the tile renderer look up the current animation frame when drawing water tiles

### Sound

 - Play sounds when selecting units
 - Play sounds when instructing units to move

### Music

 - Add support for WMA music (or convert to another format)

### Menus

 - Main menu
 - Loading screen
 - Custom Map menu
 - Pause menu
 - Troop libraries

### Editor

 - Create a ScenarioWriter class

<!----------------------------------------------------------------------------->
## Enhancements
<!----------------------------------------------------------------------------->

 - [x] Camera zooming
 - [ ] Positional audio
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
 - [x] Support for higher frame rates
 - [ ] Support for larger screen resolutions
 - [ ] Support for bigger maps
 - [ ] High-res fonts
 - [ ] Upscale graphics using AI?
 - [ ] Cross-platform releases (e.g. Raspberry Pi)

<!----------------------------------------------------------------------------->
## Project
<!----------------------------------------------------------------------------->

 - Build for Linux
 - Add script to build from command line
 - Add script to run tests from command line
 - Remove `SDL_image` dependency (if it remains unused)

<!----------------------------------------------------------------------------->
## Unit Tests
<!----------------------------------------------------------------------------->

 - Mock Shaders / ShaderUtils in tests instead of mocking so many library functions?

<!----------------------------------------------------------------------------->
## Tech Debt
<!----------------------------------------------------------------------------->

### Assets

 - Consolidate "objects_*.tga" spritesheets
    - Palisade, etc. look the same regardless of map type
    - Could combine them all into one texture

### Sound

 - Initialise 'n' sound sources up-front, and find the next available one
    when playing a sound
 - Generate a buffer for each WaveFile up front and store them in a map
    instead of creating a buffer whenever we play a sound
 - Delete all sound sources / buffers when exiting
 - Set listener position based on camera

### Music (WMA / other)

 - Convert WMA to OGG
 - Load OGG files
 - Add support for [playing OGG files](https://indiegamedev.net/2020/02/25/the-complete-guide-to-openal-with-c-part-2-streaming-audio/)

### Music (MIDI)

 - Defer loading MIDI files until needed to speed up initial load time
 - Use high-precision timers for MIDI playback

### ScenarioReader

 - Use BinaryFileReader
 - Improve error handling
 - Analyse skipped sections; do they vary? Are they read?
 - Consider rewriting ScenarioReader and ScenarioBuilder as free-standing functions, e.g.
        auto scenario_desc = readScenario(scenarioPath);
        auto scenario = buildScenario(scenario_desc);
    - These could live inside a common namespace (e.g. ScenarioUtils)

### Rendering

 - Improve palette generation
    - Separate team colours into their own arrays
    - Build a palette for each team colour dynamically
    - Render units using their correct colours (unit colours, not team colours!)
 - Framebuffer size calculations should use RenderUtils
 - Why does FramebufferRendering require a different winding order to other renderers?
 - Extract common code from SpriteRenderable and AtlasRenderable
 - Use string constants to access images within a texture atlas
 - Image keys should have sensible names (not dependent on their precise byte location since this could differ between locales)

### Design

 - Central unit type registry: animations, UI image, etc.
 - Static objects (e.g. mountains) need not use an AnimationComponent

### Refactoring

 - Sort source files into subfolders?
    - Do not auto-format library subfolders
 - Move constants from header files to CPP files?
 - Consider using std::unordered_map instead of std::map
