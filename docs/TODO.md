# To Do

<!----------------------------------------------------------------------------->
## Next Up...
<!----------------------------------------------------------------------------->

### Multiplayer Milestone

- Crash when closing game window (sometimes) (OpenAL32.dll)
- Can't use unordered_maps when iteration is required due to non-deterministic behavior in multiplayer!
- Show a message when waiting for player commands
- Create subclasses of Entity, e.g. Unit, Building, Container instead of using *PropsComponent to store basic properties

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

### Game

- Some animations need attention (e.g. Snake)
- "Vehicles" should not have an inventory
- Zooming in does not zoom towards the cursor as much as it should
- Flying units need a higher z-position so that they appear on top of units below them
- Tiles covered by the map border should not be passable
- Units can walk through buildings
- Drag-select cursor / outline should remain active when shrunk down to a small size

### Setup

- objects_meadow.tga cannot be opened in Photoshop (invalid file format)

<!----------------------------------------------------------------------------->
## Features
<!----------------------------------------------------------------------------->

### General

- Add support for [Unicode filenames](http://utf8everywhere.org/)
- Redirect stdout to log file
- Save config.json / log files to AppData (or at least provide the option)

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

- Send a checksum periodically to check that players are in-sync?
- GameState should reject new players
- Ensure we are iterating over entities / components deterministically so that pathfinding outcomes are consistent!
- Create standalone project to run dedicated server

### Mouse Picking

- Different units should have different hitboxes sizes
- Flying units need a hitbox offset

### Input

- Panning with the mouse at level edges
- Panning with middle-mouse button (drag)
- Smooth panning with the arrow keys (using acceleration)
- Panning speed should depend on the zoom level

### Fonts

- Vanilla text seems more saturated because it uses darker pixels instead of translucency
    - We should support 2 options:
        - Vanilla: Uses premade bitmaps with no translucency
        - Smooth: The current implementation
- Include shareware version of Procopius?

### Rendering

- Respect monster color
- Render interface
    - Resource icons
    - Resource counts
    - Unit ID
    - Unit stats
- Render minimap
- Render info points
- Render chests
- Respect Gold tile variants
- Fog of war
- Boats should bob when stationary

### Selection & Movement

- Selected units should flash orange
- Allow buildings to be selected
- Use a single MoveCommand for groups?
- Show "star" effect when sending troops somewhere
- Passability debug visualisation
- Pathfinding debug visualisation

### Animations

- Attack animations are missing in units.json
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

### Music

- Add support for WMA music (or convert to another format)
    - [OGG support](https://indiegamedev.net/2020/02/25/the-complete-guide-to-openal-with-c-part-2-streaming-audio/)

### Audio

- Clean up audio on exit
    [ALSOFT] (WW) 1 Source not deleted
    [ALSOFT] (WW) 1 Buffer not deleted

### Menus

- Gold cursor in menus
    - We may need to create new cursor images for this since the existing ones all use the wrong palette
- Main menu
- Loading screen
- Custom Map menu
- Pause menu
- Troop libraries
- Lobby screen

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
- [x] Stable net play!

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
- [ ] Translucent unit shadows (instead of solid black)
- [ ] Real translucency for fog of war
- [ ] Upscale graphics / videos using AI?

### Menus

- [x] Animated cursors
- [ ] Rank explanations
- [ ] Campaign map select
- [ ] Revamped Save / Hire Troops UI
- [ ] Show map preview / description
- [ ] More endgame stats
- [ ] Show additional unit stats (speed, hit speed, etc.)
- [ ] Hover states for buttons#
- [ ] Scroll wheel support for lists

### Balance

- [x] Improved pathfinding
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

### General

- Commit upscaled video files (Git LFS)
- Consider moving some docs to GitHub wiki
- Consider replacing SDL with GLFW
    - Replace SDL_image with stb_image? (this is what it uses internally)
- Setup program should log output to a file (run the script via a BAT file?)
- Treat linker warnings as errors
- Suppress warnings from JSON and spdlog libraries

### CMake

- Make some settings common to all projects (e.g. Unicode)
- Add shaders to project
- Why do additional library directies also have a `/$(Configuration)` variant?
- Include libraries in a better way
    - https://stackoverflow.com/a/61708554/1624459
    - https://github.com/g-truc/glm/blob/master/manual.md#-15-finding-glm-with-cmake
- Copy required files to build directory automatically (`build\projects\Open-Rival`)
    - DLLs
    - `res` folder
    - `config.json`
    - `args.json` (if present)
    - `vcxproj.user` (if present)
- Use auto formatter: https://github.com/cheshirekow/cmake_format
- Code review suggestions: https://codereview.stackexchange.com/questions/286277/a-slightly-unconventional-cmake-project
- Document CMake options
- Document using CMake from command line

### Tools

- Make scripts work with ALL projects
    - move_file.py only fixes references in Open-Rival
    - new_file.py and gen_project_files.py can only be used for Open-Rival
- Add script to prepare the `dist` folder (see the [release checklist](release_checklist.md))
- Add IWYU to build pipeline: https://include-what-you-use.org/

<!----------------------------------------------------------------------------->
## Unit Tests
<!----------------------------------------------------------------------------->

### General

- Lower warning level for test project
- Refactor Open-Rival code such that fewer mocks are needed
- Use EntityFactory to create entities for tests
- Tests should use a custom config file with debugging enabled
- Document how to run a specific test

### Entity/Component Lifecycle

- Test that EntityComponents get deleted when an Entity is deleted
- Test requestAddEntity + addPendingEntities

### Movement

- Test setting a new route before the old one is finished

<!----------------------------------------------------------------------------->
## Tech Debt
<!----------------------------------------------------------------------------->

### Assets

- Consolidate "objects_*.tga" spritesheets
    - Palisade, etc. look the same regardless of map type
    - Could combine them all into one texture
- Add padding between spritesheet images to prevent any texure bleeding

### Fonts

- How do we calculate the font size correctly?
    - https://stackoverflow.com/questions/68976859/how-to-calculate-size-of-windows-bitmap-font-using-freetype

### Optimisation

- Only mouse-pick objects that were rendered last frame
- Static objects (e.g. mountains) need not use an AnimationComponent
- Differentiate between components that need to be ticked and those that don't
- Scenario should maintain a list of entities as well as a map, instead of rebuilding the list whenever it's needed
- Hitbox buffers should not need to be recreated every tick
- EntityFactory should use maps instead of switch statements
- Use a char[] or a std::vector<uninitialized_char> for buffers to avoid initialising elements
    - https://stackoverflow.com/questions/11149665/c-vector-that-doesnt-initialize-its-members
- The palette texture does not need an alpha channel
- Defer loading MIDI files until needed to speed up initial load time
- Eliminate branching in shaders
- Revise `World::getEntityAt` to use a map or spartial partitioning

### Portability

- Read/write fixed-value types to packets instead of int, size_t, etc.
- Read/write values to packets using a consistent endianness
    - "Before writing to any WinSock structure, always convert from host order to network order,
       and after reading always convert from network order to host order"
- Add Linux implementations in platform folder
- Move existing platform-specific functionality to platform folder and add Linux implementation:
    - Default font directories
    - Registry operations in setup project

### Refactoring

- Separate Resources from resource *loading*
    - In tests, we only need to mock resource loading
- Use some kind of "magic enum" library for enum-to-string functionality
- Prefer default member initialization to initializer lists
- Don't use `const` or references for member variables?
    - Should Resources always be `const` (e.g. when using `app.getResources()`)?
    - "Unhandled error during initialization or gameplay" - would be nice to split into 2 separate stages
- Remove BinaryFileReader in favour of FileUtils::readBinaryFile
    - Add a new class that can read the buffer and maintain some offset
- Use a common file reading mechanism in audio-extractor
- Use RAII to handle setting / resetting of OpenGL flags (see GLUtils::PackAlignment)
- Avoid static methods
    - Use a namespace for public methods
    - Put private methods in a `.cpp` file
- Improve error handling (ensures, etc.)
    - Pointer access and map access is fragile
- Use client-attourney pattern or add checks to prevent misuse of add/removeEntity and addPendingEntities
- Use vectors in MousePicker instead of separate x/y variables
- Can `getComponent<MyComponent>(MyComponent::key)` be simplified somehow?
- Replace `Resources::get*Spritesheet` with a generic method that takes an enum parameter
- Create a CursorRenderer to hold all the cursor rendering logic
- Reduce duplication between Unit/BuildingAnimationComponent
- Reduce duplication between Unit/BuildingDef
- Use building definitions instead of hardcoded logic for building properties: `getWidth`, `getHeight`, `isWall`
- UnitDef exposes some properties as public, which could be dangerous
- PassabilityComponent for units is only used to set the initial passability; after that, we rely on the MovementComponent
- Replace ConfigUtils with JsonUtils
- Improve enums: https://stackoverflow.com/review/suggested-edits/33596043
    - Also see comment: "You don't need `typename` here"
- Copy/move constructors should use `noexcept`
- Allow non-copyable classes to be moved (check uses of "= delete")
- Replace PacketFactory's switch statement with a map of type -> function
- Duplication between GameState::pollNetwork and LobbyState::pollNetwork
- UiImage should be split into 3 different classes
- Move some headers to pch.h
- Don't return references
- Run cppcheck
- PassabilityComponent is pointless for units, this should be handled by the MovementComponent instead
- Allow MapNodes to be logged using spdlog: https://github.com/gabime/spdlog/wiki/1.-QuickStart#log-user-defined-objects

#### Rendering

- Framebuffer size calculations should use RenderUtils
- Extract common code from SpriteRenderable and AtlasRenderable
- Use string constants to access images within a texture atlas
- Font shader could accept 2d vertices since z is always 0
- Some vertex/index data can be made into constants instead of member variables
    - Even the menu view-projection matrix can probably be reused each frame
- In debug mode, check `programId` is non-zero when using a shader
- Rendering code is super messy
    - Create a utility method to construct a vertex array from x1,x2,y1,y2,z positions
    - Encapsulate logic in classes (e.g. UiImage) where possible instead of duplicating code
- Renderables should be moveable (like TextRenderable)

#### ScenarioReader

- Use BinaryFileReader
- Improve error handling
- Analyse skipped sections; do they vary? Are they read?
- Consider rewriting ScenarioReader and ScenarioBuilder as free-standing functions, e.g.
        auto scenario_desc = readScenario(scenarioPath);
        auto scenario = buildScenario(scenario_desc);
    - These could live inside a common namespace (e.g. ScenarioUtils)

#### Sound

- Initialise 'n' sound sources up-front, and find the next available one when playing a sound
- Generate a buffer for each WaveFile up front and store them in a map instead of creating a buffer whenever we play a sound
- Delete all sound sources / buffers when exiting

#### UI

- UI rendering is a total mess
    - Create a hierarchical UiElement class
