# To Do

> Also see [milestones](/docs/milestones.md).

<!----------------------------------------------------------------------------->
## WIP
<!----------------------------------------------------------------------------->

### Movement

- Facing is not updated during movement
    - `prepareNextMovement` is only called once per route
- WalkerComponent should not be responsible for animations
    - Perhaps UnitPropsComponent should broadcast the new state to the AnimationComponent

### Data Loading

- Central unit type registry: animations, UI image, etc.
    - Data is being loaded from json but should be deserialised into a more useable object
    - Hardcoded animations should be removed

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

- Iterators may break if an Entity is added from inside the loop
- Game will crash if no config file exists

<!----------------------------------------------------------------------------->
## Features
<!----------------------------------------------------------------------------->

### General

- Custom cursors
- Logging mechanism
    - Log important events to file
    - Replace cout / cerr with logger calls
    - texture-builder is particularly noisy
- Add support for [Unicode filenames](http://utf8everywhere.org/)

### Data Loading

- Some bytes of `Interfac.dat` are still unknown
    - These are probably not too important but some may relate to cursor offsets
    - Currently some cursor images are strangely offset and will need fixing
- Pack interface images into textures
- Finish parsing Goals
- Finish parsing Chests (contents)
- Include missing alphabet entries (e.g. '!{})
- Test resource extraction from other locales
- Try reading the original font files

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

- Write server code that can forward messages between clients
    - This should exist in a standalone project so that the server can be run independently
    - Open-Rival should import this so that one player can act as a host
- Clients should connect to server using TCP
- Use a lock-step model
    - See [multiplayer notes](/docs/multiplayer.md)
    - https://www.informit.com/articles/article.aspx?p=2461064&seqNum=3
    - https://gafferongames.com/post/what_every_programmer_needs_to_know_about_game_networking/
- Clients schedule commands at 'n' ticks in the future
- Each client runs a deterministic simulation of the game by applying commands from all players each tick

### Input

- Panning with the mouse at level edges
- Panning with middle-mouse button (drag)
- Smooth scrolling with the arrow keys
- Scroll speed should depend on the zoom level
- Drag-select

### Fonts

- Vanilla text seems more saturated because it uses darker pixels instead of translucency
    - We should support 2 options:
        - Vanilla: Uses premade bitmaps with no translucency
        - Smooth: The current implementation
- Include shareware version of Procopius?

### Rendering

- Render units "between" logical frames based on delta values
- Render interface
    - Render resource icons
- Render minimap
- Render info points
- Render chests
- Respect unit color
- Respect Gold tile variants
- Fog of war

### Movement

- Clicking should plan a route for the selected unit(s)
    - Specifically, this should issue a MoveCommand that sets the route
    - Ensure we are iterating over entities / components deterministically so that pathfinding outcomes are consistent!
- Units should periodically re-plan their route
- Units should stop moving if the path becomes blocked
- Units should modify tile passability as they move
- Should units "try" to move somewhere even if there is no path?
- Should we limit the pathfinding logic to 'n' iterations?
- Support flying units
- Support seafaring units

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
- [ ] Stable net play!

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
- [ ] Using abilities with a group selected
- [x] High-res fonts
- [ ] Allow troops to stop moving if they are dying to traps
- [ ] Repairing allied siege units
- [ ] Real translucency for fog of war
- [ ] Upscale graphics / videos using AI?

### Menus

- [ ] Rank explanations
- [ ] Campaign map select
- [ ] Revamped Save / Hire Troops UI
- [ ] Show map preview / description
- [ ] More endgame stats
- [ ] Show additional unit stats (speed, hit speed, etc.)
- [ ] Hover states for buttons

### Balance

- [ ] Improved pathfinding
- [ ] Configurable Gold multiplier
- [ ] Configurable XP multiplier
- [ ] Affordable Wall
- [ ] Non-suicidal peasants (don't go looking for gold)
- [ ] Rebalance abilities (Frozen Breath, Capture)
- [ ] Disable friendly fire

### New Features

- [ ] Positional audio
- [ ] Bigger unit inventories (just for fun!)
- [ ] Multiplayer campaigns
- [ ] Support for bigger maps
- [ ] Replays
- [ ] Random map generation
- [ ] Placing beacons to alert teammates

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
- Improve handling of libary files
    - Suppress warnings around includes rather than editing library files
    - Move library headers (json, RtMidi) into a separate project or folder?
    - Clearly annotate any changes to library files
- Suppress warnings from SDL (C26819, C26812)

<!----------------------------------------------------------------------------->
## Unit Tests
<!----------------------------------------------------------------------------->

- Write a tool to automatically generate `Open-Rival-test.proj`
    - Copy all includes from Open-Rival
    - Copy all source files from Open-Rival (except those we have stubbed)
- Mock Shaders / ShaderUtils in tests instead of mocking so many library functions?

<!----------------------------------------------------------------------------->
## Tech Debt
<!----------------------------------------------------------------------------->

### Assets

- Fix "libpng warning: iCCP: known incorrect sRGB profile""
- Consolidate "objects_*.tga" spritesheets
    - Palisade, etc. look the same regardless of map type
    - Could combine them all into one texture
- Add padding between spritesheet images

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

### Fonts

- How do we calculate the font size correctly?
    - https://stackoverflow.com/questions/68976859/how-to-calculate-size-of-windows-bitmap-font-using-freetype

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

- Static objects (e.g. mountains) need not use an AnimationComponent
- Differentiate between components that need to be ticked and those that don't
- Scenario should maintain a list of entities as well as the map, instead of rebuilding the list whenever it's needed

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
- Pass small interfaces around instead of complex objects
    - e.g. Resources should extend TextureStore, AudioStore, etc.
- Prefer forward-declarations to includes, to reduce compile times
- EntityFactory should use maps instead of switch statements
- Use #pragma once?
- Improve error handling (ensures, etc.)
    - Pointer access and map access is fragile
- Always initialize properties
