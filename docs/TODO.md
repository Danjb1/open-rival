# To Do

<!----------------------------------------------------------------------------->
## Next Up
<!----------------------------------------------------------------------------->

### Combat

- Ensure all units are using the correct death effect and death sound
- When a unit dies, refresh the player context (update group leader portrait, end attack mode if dead, etc.)
- Attacks (projectiles) with splash damage should nearby damage units
- Projectile impact / explosion graphics

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

- Some animations need attention (e.g. Snake)
- "Vehicles" should not have an inventory
- Zooming in does not zoom towards the cursor as much as it should
- Flying units need a higher z-position so that they appear on top of units below them
- Flying units should play their death effect higher up
- Tiles covered by the map border should not be passable
- When the game is paused (at a breakpoint) it can take a long time to resume / catch up (when playing sounds?)
- Attacking units are slow to respond to MoveCommands (e.g. very noticeable when their target moves away)
- Cursor sometimes resets to default when one of the selected units dies
- Occasional desyncs when moving/attacking with groups in multiplayer
- Sometimes death sound plays multiple times in multiplayer (once per attacking unit?)
- We need to rework `getMutableEntityAt` because one tile can contain *multiple* entities!
- Ranged units sometimes keep attacking after the target dies
- Trying to attack an empty tile with a ranged unit just moves there instead

<!----------------------------------------------------------------------------->
## Features
<!----------------------------------------------------------------------------->

### Spatial Partitioning

- Modify methods in World to allow efficient retrieval of entities
- Divide the world into a grid and have a map of cell -> entities
    - Only check the relevant cells when retrieving entities
    - Whenever an entity moves, spatial partitioning should handle moving entities between cells
- World should handle spatial partitioning internally; caller does not care about the details
- Can also store lists of entities by type in World for quick filtering (e.g. get only units)

### Movement

- Ground units should be able to traverse thin strips of water, and NOT sea units - test this!

### Combat

- Units should automatically target nearby enemies (needs spatial partitioning!)
    - Respect "Fighting Area" - do not wander too far
- Units should rest when idle (approx. 200 health in 30 seconds)
- Allow attacking buildings
- Allow attacking an empty tile when in attack mode (ranged attacks only!)
    - For melee attacks it should display a message "Could not attack there !"
- Destroying cropland
- For flying units, projectiles also need to lerp "downwards"?
- Scale damage based on the "Fighting Speed" setting
    - The Fighting Speed slider goes from 0.5 to 2.0
    - At 0.5, all damage is halved, and at 2.0, all damage is doubled
    - It is not possible to set Fighting Speed to exactly 1! The default value is around 0.875
- Don't kill friendly units when attacking them!

### Input & Mouse Picking

- Panning with the mouse at level edges
- Panning with middle-mouse button (drag)
- Smooth panning with the arrow keys (using acceleration)
- Panning speed should depend on the zoom level
- Different units should have different hitboxes sizes
- Flying units need a hitbox offset
- Ctrl+click to select all units of type
- Shift+click/drag to add more units to a selection
- Shift+click a selected unit to remove it from the selection

### HUD & Menus

- Revamp [menu system](/docs/design/menus.md)
- Show resources in HUD
- Show unit ID in HUD
- Show unit stats in HUD
- Show group info in HUD when multiple units are selected
- Expand HUD to fit larger screen resolutions
- On-screen message display (e.g. "We are under attack!")
- Main menu
- Loading screen
- Custom Map menu
- Pause menu
- Settings menu
- Spybook
- Postgame screen
- Troop libraries
- Lobby screen
- Campaign text
- Credits

### Town Building

- Allow buildings to be selected
    - Play a sound
- Building construction
    - Buildings must be placed within 6 tiles of an existing building

### Items

- Units should have an InventoryComponent
    - Shipyards must be placed on a coastline
- Resource gathering
- Training new units
- Food consumption
- Selling buildings
- Repairing buildings
- Picking up and dropping items
- Exchanging items with another unit
- Items should affect unit stats
- Some items can be used (e.g. scrolls, keys)
- Units should drop their items on death

### Experience & Levelling

- Award experience when attacking
- Increase max health when attacking
- Show experience levels in the HUD
- Gain items when levelling up
- Unlock upgrades when levelling up

### Fog & Minimap

- Render darkness
- Render fog of war
- Uncover fog based on sight
- Render minimap
- Click on minimap to move the camera
- Fog is temporarily revealed in a small radius when a unit fires a projectile from out of sight

### Scenarios & Game Objects

- Scenario file should be able to override unit stats
- Scenario file should be able to override building stats
- Scenario file should be able to override attack definitions
- Win / loss conditions (goals)
- Mountains
- Scenery
- Chests
- Info Points
- Doors
- Prisoners
- Mercenaries
- Campaigns

### Upgrades & Spells

- Show upgrades in the HUD
    - Overlay background gets taller for hovered units with upgrades
    - Upgrade rectangles use 0xffffff for top/left edge, 0x616161 for bottom/right edge and 0xa2a2a2 in the middle
- Register additional attacks when the relevant upgrades are unlocked
    - Pick the most suitable attack when a new target is set
    - Implement per-attack cooldowns
- Sending units to get upgraded
- Some upgrades should modify unit stats
- Mana bar and mana consumption
- Magic shield
- Repairing vehicles
- Thieving
- Invisibility
- Spying
- Traps
- Bombs
- Place Land/Water ability
- Defense (Watchtowers / town centers)
- Implement spells
- Allow spells to be set as the default attack

### Monsters

- Monsters should use the correct color
- Monsters should use attacks based on their color

### Miscellanous

- Starting locations (initial camera location per-player)
- Transporting units in vehicles
- Shooting from vehicles (toggleable game option)
- Click on health bar to go get healed / repaired
- Unit groups (Ctrl + 1-9)
- Units should rotate randomly when idle (except vehicles)
- Teams
    - Shared fog of war
    - Flagging locations to allies
    - Transferring peasants / resources to allies
- Changing unit colours
- Formations, Patrol and Stand Ground (see manual)
- Replays
- Chat
- Cheat codes
- Savegames

### Visuals

- Units with more than 2 bars should display a plus sign in their overlay
- Health bars should be rendered higher for flying units
- Selected units should flash orange
- Trees should animate (but not all in-sync)
- Respect Gold tile variants
- Gold tile animations
    - Each frame, call `Scenario.update`
    - This should pick 20 tiles randomly from those that are visible
    - Any gold tiles that get picked should be animated:
        - Keep a list of goldTileAnimations
        - Add an entry to this list
        - Re-use logic from AnimationComponent
    - Add an extra step to the TileRenderer to render these animations on top of the relevant gold tiles
- Show "star" effect when sending troops somewhere
- Boats should bob when stationary

### AI Players

- Building
- Resource gathering
- Training units
- Exploration
- Combat
- Transporting units

### Audio & Video

- Play the right music tracks based on context
- Play intro video
- Play campaign videos
- Add support for WMA music (or convert to another format)
    - [OGG support](https://indiegamedev.net/2020/02/25/the-complete-guide-to-openal-with-c-part-2-streaming-audio/)
- Use seeded randomizer for SoundBanks so all players hear the same audio? (be careful if this happens on another thread!)

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
- [ ] Modern RTS controls (right-click to move, double-tap group key to center)
- [ ] Easy military drag-select
- [ ] Rally points
- [ ] Find idle workers
- [ ] Queue traps for placement
- [ ] Show mana bars in unit tooltips
- [ ] Using abilities with a group selected
- [x] High-res fonts
- [ ] Allow troops to stop moving if they are dying to traps
- [ ] Repairing allied siege units
- [x] Remove 25-unit limit for selections
- [ ] Allow AI players to be allied

### Assets

- [ ] Translucent unit shadows (instead of solid black)
- [ ] Real translucency for fog of war
- [ ] Upscale graphics / videos using AI?
- [ ] Remastered audio

### Menus

- [x] Animated cursors
- [ ] Rank explanations
- [ ] Campaign map select
- [ ] Revamped Save / Hire Troops UI
- [ ] Show map preview / description
- [ ] More endgame stats
- [ ] Show additional unit stats (speed, hit speed, etc.)
- [ ] Hover states for buttons
- [ ] Scroll wheel support for lists

### Balance

- [x] Improved pathfinding
- [ ] Configurable Gold multiplier
- [ ] Configurable XP multiplier
- [ ] Affordable Wall
- [ ] Non-suicidal peasants (don't go looking for gold)
- [ ] Rebalance abilities (Frozen Breath, Capture)
- [ ] Disable friendly fire from ranged attacks

### New Features

- [ ] Positional audio
- [ ] Bigger unit inventories (just for fun!)
- [ ] Multiplayer campaigns
- [ ] Support for bigger maps
- [ ] Replays
- [ ] Random map generation
- [ ] Placing beacons to alert teammates
- [ ] Button on units to show lore from manual
- [ ] Achievements

### Map Editor

- [ ] Revamped map editor
- [ ] Campaign editor

<!----------------------------------------------------------------------------->
## Project
<!----------------------------------------------------------------------------->

### General

- Consider moving some docs to GitHub wiki
- Consider replacing SDL with GLFW
    - Replace SDL_image with stb_image? (this is what it uses internally)
- Treat linker warnings as errors
- Suppress warnings from JSON and spdlog libraries
- Do not use header-only JSON library as it leads to longer compile times

### CMake

- Make some settings common to all projects (e.g. Unicode)
- Why do additional library directories also have a `/$(Configuration)` variant?
- Include libraries in a better way
    - https://stackoverflow.com/a/61708554/1624459
    - https://github.com/g-truc/glm/blob/master/manual.md#-15-finding-glm-with-cmake
- Copy required files to setup build directory automatically (`build\x64\projects\setup`)
    - `setup` folder
    - `res` folder also needs to exist!
- Resources, DLLs, config file, etc. are not checked for changes when running ZERO_CHECK
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

### Setup

- Setup utilities should log debug output to a file (currently they use cout)
- Graphical setup utility
- Ability to re-run individual steps of the setup program

### Assets

- Consolidate "objects_*.tga" spritesheets
    - Palisade, etc. look the same regardless of map type
    - Could combine them all into one texture
- Create (animated?) gold cursor for menus

### Debugging

- Passability debug visualisation
- Pathfinding debug visualisation

### Fonts

- How do we calculate the font size correctly?
    - https://stackoverflow.com/questions/68976859/how-to-calculate-size-of-windows-bitmap-font-using-freetype
- Try reading the original font files
- Vanilla text seems more saturated because it uses darker pixels instead of translucency
    - We should support 2 options:
        - Vanilla: Uses premade bitmaps with no translucency
        - Smooth: The current implementation

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
- AtlasRenderables always use a z co-ordinate even though it is often not needed (e.g. UI, overlays)
- Use emplace with std::piecewise_construct instead of insert where possible for maps
- When pathfinding with a group, prioritise visiting nodes that were recently visited by other group members
- When pathfinding with a group, limit the search based on how long the first group member took to find a route
- Use pathfinding context when attack-moving a group
- Pathfind from the destination to the start
- Analyse the map to determine which areas are distinct and unreachable from each other
- Use [heirarchical pathfinding](https://factorio.com/blog/post/fff-317)
- Optimise MapUtils::findNeighbors
- getEntitiesInArea - for finding entities in the camera / drag-select Rect

### Portability

- Test resource extraction from other locales
- Save config.json / log files to AppData (or at least provide the option)
- Add support for [Unicode filenames](http://utf8everywhere.org/)
- Read/write fixed-value types to packets instead of int, size_t, etc.
- Read/write values to packets using a consistent endianness
    - "Before writing to any WinSock structure, always convert from host order to network order,
       and after reading always convert from network order to host order"
- Add Linux implementations in platform folder
- Move existing platform-specific functionality to platform folder and add Linux implementation:
    - Default font directories
    - Registry operations in setup project

### Networking

- Adjust net command delay dynamically based on ping
    - If delay gets increased, clients should send empty commands for any "skipped" ticks
    - If delay gets reduced, clients keep issuing commands for the next tick that was due, until the "current" tick catches up
- Send a checksum periodically to check that players are in-sync? (debug mode only)
- GameState should reject new players
- Create standalone project to run dedicated server

### Refactoring

#### General

- In tests, we only need to mock resource *loading*, but not Resources
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
- LobbyState's mechanism for passing unprocessed packets back to the connection is hacky
    - Connection could store receivedPackets instead, and return an iterator
- GameState should expose an iterator instead of making a copy of the entity list in getEntities
- Provide an easier way to get components that are guaranteed to exist for a Unit
- Revise usages of weakThis in EntityComponents (use std::enable_shared_from_this<EntityComponent>)
- Some properties (e.g. Name) should be shared by Units and Buildings (create a struct?)
- Unit doesn't need to hold weak_ptrs since it outlives its components
- Units sometimes take a suboptimal route (see note about `nodesToAvoid`)
- Catch exceptions when calling `JsonUtils::readJsonFile`
- Use a std::span to enforce immutability of a container (instead of duplicating logic in World::getEntitiesInRadius)
- Improve compile times!
    - Move some stuff out of headers, e.g. constants in ResourceLoader

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

- Separate music from AudioSystem
- AudioSystem should not be "destroyed" when sound is disabled, because it could be re-enabled at any point
    - We should stop all sounds but keep the buffers
- Initialise 'n' sound sources up-front, and find the next available one when playing a sound
- Playing sounds is quite fiddly (need both AudioStore and AudioSystem)
