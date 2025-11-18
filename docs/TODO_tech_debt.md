# Tech Debt

<!----------------------------------------------------------------------------->
## Project
<!----------------------------------------------------------------------------->

### General

- Consider moving some docs to GitHub wiki
- Treat linker warnings as errors

### Dependencies

- Is GLEW actually needed?
- Consider replacing SDL with GLFW
    - Replace SDL_image with stb_image? (this is what it uses internally)
- Suppress warnings from JSON and spdlog libraries
- Do not use header-only JSON library as it leads to longer compile times

### CMake

- Make some settings common to all projects (e.g. Unicode)
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
- Create unit components from JSON?

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
