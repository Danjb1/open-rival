# To Do

> Also see [milestones](/docs/milestones.md).

<!----------------------------------------------------------------------------->
## WIP
<!----------------------------------------------------------------------------->

### Movement Milestone

- Animate cursor
- Show a custom cursor when the mouse is over a unit
- Add all unit voices and portraits
- Support flying units
- Support seafaring units
- Drag-select
- Build and release!
    - Setup programs may be broken by recent dependency changes
    - Setup program should log output to a file (run the script via a BAT file?)

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

- Game will crash if no config file exists
- Clicking in the UI will still move the selected unit (to the last valid mouse position)
- Zooming in does not zoom towards the cursor as much as it should

<!----------------------------------------------------------------------------->
## Features
<!----------------------------------------------------------------------------->

### General

- Logging mechanism
    - Log important events to file
    - Replace cout / cerr with logger calls
    - texture-builder is particularly noisy
- Add support for [Unicode filenames](http://utf8everywhere.org/)

### Data Loading

- Some bytes of `Interfac.dat` are still unknown
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

- Ensure we are iterating over entities / components deterministically so that pathfinding outcomes are consistent!
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

### Mouse Picking

- Different units should have different hitboxes sizes
    - Add option to render hitboxes for debugging
- Flying units need a hitbox offset

### Input

- Panning with the mouse at level edges
- Panning with middle-mouse button (drag)
- Smooth panning with the arrow keys
- Panning speed should depend on the zoom level

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
- Respect Gold tile variants
- Fog of war
- Some vertex/index data can be made into constants instead of member variables
    - Even the menu view-projection matrix can probably be reused each frame

### Movement

- Units should periodically re-plan their route
- Should units "try" to move somewhere even if there is no path?
- Should we limit the pathfinding logic to 'n' iterations?

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
- [ ] Modern RTS controls (right-click to move)
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

- [ ] Animated cursors
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
- Use a dependency manager
- Add script to build from command line
- Add script to run tests from command line
- Add script to prepare the `dist` folder (see the [release checklist](release_checklist.md))
- Use a separate .gitignore file for each project folder
- Commit upscaled video files (using Git LFS?)
- Consider linking with static libraries (instead of using DLLs)

<!----------------------------------------------------------------------------->
## Unit Tests
<!----------------------------------------------------------------------------->

### General

- Write a tool to automatically generate `Open-Rival-test.proj`
    - Copy all includes from Open-Rival
    - Copy all source files from Open-Rival (except those we have stubbed)
- Mock Shaders / ShaderUtils in tests instead of mocking so many library functions?

### Entity/Component Lifecycle

- Test that EntityComponents get deleted when an Entity is deleted
- Test requestAddEntity + addPendingEntities

### Movement

- Test setting a new route before the old one is finished

<!----------------------------------------------------------------------------->
## Tech Debt
<!----------------------------------------------------------------------------->

### Assets

- Fix "libpng warning: iCCP: known incorrect sRGB profile""
- Consolidate "objects_*.tga" spritesheets
    - Palisade, etc. look the same regardless of map type
    - Could combine them all into one texture
- Add padding between spritesheet images to prevent any texure bleeding

### Sound

- Initialise 'n' sound sources up-front, and find the next available one when playing a sound
- Generate a buffer for each WaveFile up front and store them in a map instead of creating a buffer whenever we play a sound
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
- Extract common code from SpriteRenderable and AtlasRenderable
- Use string constants to access images within a texture atlas
- Font shader could accept 2d vertices since z is always 0

### Optimisation

- Only mouse-pick objects that were rendered last frame
- Static objects (e.g. mountains) need not use an AnimationComponent
- Differentiate between components that need to be ticked and those that don't
- Scenario should maintain a list of entities as well as the map, instead of rebuilding the list whenever it's needed

### Refactoring

- UI rendering is a total mess
    - Create a hierarchical UiElement class
- Use some kind of "magic enum" library for enum-to-string functionality
- Prefer default member initialization to initializer lists
- Don't use `const` or references for member variables?
    - Should Resources always be `const` (e.g. when using `app.getResources()`)?
- Add "DEBUG" macro variable that performs additional checks if set
    - Ensure `programId` is non-zero when using a shader
- Sort source files into subfolders?
- Remove BinaryFileReader in favour of FileUtils::readBinaryFile
    - Add a new class that can read the buffer and maintain some offset
- Use a common file reading mechanism in audio-extractor
- Use RAII to handle setting / resetting of OpenGL flags (see GLUtils::PackAlignment)
- Avoid static methods
    - Use a namespace for public methods
    - Put private methods in a `.cpp` file
- EntityFactory should use maps instead of switch statements
- Improve error handling (ensures, etc.)
    - Pointer access and map access is fragile
- Use client-attourney pattern or add checks to prevent misuse of add/removeEntity and addPendingEntities
- Use vectors in MousePicker instead of separate x/y variables
- Can `getComponent<MyComponent>(MyComponent::key)` be simplified somehow?
- Create subclasses of Entity, e.g. Unit, Building, Container instead of using *PropsComponent to store basic properties
