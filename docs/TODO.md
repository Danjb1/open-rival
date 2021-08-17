# To Do

<!----------------------------------------------------------------------------->
## WIP
<!----------------------------------------------------------------------------->

 - Font rendering
    - [TextRenderable] Support changing text (may need to expand buffers!)
    - What font size should we use when we create the font texture?
        - This has widespread ramifications as the scale used in rendering
            depends on the texture size! Can we separate the two?
        - `scale` should be a factor of the "default text height" rather than
            the texture size.
        - We should calculate the ACTUAL scale to render at based on:
            scale * defaultTextHeight / fontTexHeight
        - So if scale = 1, defaultTextHeight = 14 and fontTexHeight = 48,
            we will actually render at 0.29 scale
        - The space width should be set based on this default text size
        - 16 or 18 looks good as a default
            - Compare to original game
            - Set fontTexHeight so that it divides nicely / gives a nice result!
    - Load all fonts that we need on load and store them somewhere
    - Load the font directory / font names from a config file
    - Free font textures on exit

 - Improve setup project
    - Interface extractor: some bytes are still unknown
    - Filenames can get truncated due to limited size of char array
        - extractImages() method in setup/src/image-extractor.cpp assumes at most 63 character filename (including path)
        - Don't use snprintf!

 - Improve Images
    - Image ctor is cumbersome: std::make_unique<std::vector<std::uint8_t>>(data)
    - Rather than supporting Images with a custom stride, we should just realign the data in image-extractor once we know the image size
    - Image needn't always use a unique_ptr to hold data; sometimes we just need a temporary Image which can live on the stack (e.g. fonts)
        - We could create an Image interface and 2 subclasses: StackImage / HeapImage

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

 - Do iterators break if an Entity is added from inside the loop?
 - Game will crash if no config file exists

<!----------------------------------------------------------------------------->
## Features
<!----------------------------------------------------------------------------->

### General

 - Custom cursors
    - Images for animated cursor (in-game) are missing
 - Logging mechanism
    - Log important events to file
    - Replace cout / cerr with logger calls
    - texture-builder is particularly noisy
 - Add support for [Unicode filenames](http://utf8everywhere.org/)

### Data Loading

 - Pack interface images into textures
 - Add interface extraction to setup application
 - Finish parsing Goals
 - Finish parsing Chests (contents)
 - Include missing alphabet entries (e.g. '!{})
 - Test resource extraction from other locales

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
 - Respect unit color
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

### Compatibility

 - [ ] Multilanguage support
 - [ ] Cross-platform releases (e.g. Raspberry Pi)
 - [x] Support for higher frame rates
 - [ ] Support for larger screen resolutions

### Quality of Life

 - [ ] UI scaling
 - [x] Camera zooming
 - [ ] Smooth camera panning
 - [ ] Middle-click camera panning
 - [ ] Easy military drag-select
 - [ ] Rally points
 - [ ] Find idle workers
 - [ ] Queue traps for placement
 - [ ] Show mana bars in unit tooltips
 - [ ] Stable net play!
 - [ ] Using abilities with a group selected
 - [x] High-res fonts
 - [ ] Real translucency for fog of war
 - [ ] Upscale graphics / videos using AI?

### Menus

 - [ ] Rank explanations
 - [ ] Campaign map select
 - [ ] Revamped Save / Hire Troops UI
 - [ ] Show map preview / description
 - [ ] More endgame stats
 - [ ] Show additional unit stats (speed, hit speed, etc.)

### Balance

 - [ ] Improved pathfinding
 - [ ] Configurable Gold multiplier
 - [ ] Configurable XP multiplier
 - [ ] Affordable Wall
 - [ ] Non-suicidal peasants (don't go looking for gold)

### New Features

 - [ ] Positional audio
 - [ ] Bigger unit inventories (just for fun!)
 - [ ] Multiplayer campaigns
 - [ ] Support for bigger maps
 - [ ] Replays
 - [ ] Random map generation

### Map Editor

 - [ ] Revamped map editor
 - [ ] Campaign editor

<!----------------------------------------------------------------------------->
## Project
<!----------------------------------------------------------------------------->

 - Build for Linux
 - Add script to build from command line
 - Add script to run tests from command line
 - Auto-arrange includes via clang-format?
 - Suppress warnings around includes rather than editing library files
 - Suppress warnings from SDL (C26819, C26812)

<!----------------------------------------------------------------------------->
## Unit Tests
<!----------------------------------------------------------------------------->

 - Mock Shaders / ShaderUtils in tests instead of mocking so many library functions?

<!----------------------------------------------------------------------------->
## Tech Debt
<!----------------------------------------------------------------------------->

### Assets

 - Fix "libpng warning: iCCP: known incorrect sRGB profile""
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
    - Separate team colors into their own arrays
    - Build a palette for each team color dynamically
    - Render units using their correct colors (unit colors, not team colors!)
 - Framebuffer size calculations should use RenderUtils
 - Why does FramebufferRendering require a different winding order to other renderers?
 - Extract common code from SpriteRenderable and AtlasRenderable
 - Use string constants to access images within a texture atlas

### Design

 - Central unit type registry: animations, UI image, etc.
 - Static objects (e.g. mountains) need not use an AnimationComponent

### Refactoring

 - Add "DEBUG" macro variable that performs additional checks if set
    - Ensure `programId` is non-zero when using a shader
 - Sort source files into subfolders?
    - Do not auto-format library subfolders
 - Move constants from header files to CPP files?
 - Consider using std::unordered_map instead of std::map
 - Remove BinaryFileReader in favour of FileUtils::readBinaryFile
    - Add a new class that can read the buffer and maintain some offset
 - Use a common file reading mechanism in audio-extractor
 - Use RAII to handle setting / resetting of OpenGL flags (see GLUtils::PackAlignment)
 - Don't use `const` for member variables?
 - Avoid reference member variables (prefer pointers)?
 - Avoid static methods
    - Use a namespace for public methods
    - Put private methods in a `.cpp` file
 - List methods before fields in classes
    - Separate fields using a new public/private heading
