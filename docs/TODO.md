# To Do

## Improvements

 - TileRenderer:
    - Simplify for-loop iteration with index
    - Duplication between UnitRenderer and TileRenderer
    - Render to a texture instead of redrawing everything every frame
    - Add support for animated tiles
 - UnitRenderer:
    - Make vertex-related values into constants
    - Render Units at the correct positions (re-use code from TileRenderer)
    - Simplify for-loop iteration with index
    - Delete Renderables when a Unit no longer exists
    - Add support for team colours
 - ScenarioReader:
    - Use BinaryFileReader
    - Improve error handling
    - Finish parsing Goals
    - Finish parsing Chests (contents)
    - Test reading all built-in scenario files
    - Test reading campaign files / introductory levels
    - Analyse skipped sections; do they vary? Are they read?
    - Create a Scenario from the final struct

## Features

### Installer

 - Script to extract all assets from game directory

### General

 - Set window icon
 - Log to file
 - Static code analysis
 - Unit tests

### Editor

 - Create a ScenarioWriter class

### Image Preparation

 - Extract menu / interface images
 - Extract fonts

### Engine

 - Create data structures:
    - Map
    - TileType
    - UnitType
    - BuildingType
    - ItemType
    - GameObject
        - Unit (Ground / Flying)
        - Building
        - Obstacle
        - Door
        - Effect
 - Scene organisation / traversal
    - A single tile might have:
        - Terrain
        - Cropland
        - A ground unit
        - A flying unit
        - An effect (such as a corpse animation)
    - Ability to list all units
    - Ability to get a unit by ID
    - Ability to know which tile a given unit occupies
    - Ability to know which GameObject and flying unit occupies a tile
    - Ability to know the type of a tile
    - Ability to deduce the passability of a tile
    - Ability to list all effects
 - Game loop

### Game Logic

 - Use the Command Pattern to manipulate game state?
 - Use the lockstep model for multiplayer?
 - Moving units to adjacent tiles
 - Pathfinding
 - Building
 - Training
 - Resting
 - Attacking
 - Upgrades
 - Experience
 - Food
 - Items
 - Spells
 - Monster AI
 - Player AI

### Input

 - Selecting units (render to a texture with mouse picking shader)
 - Drag-select

### Rendering

 - Camera class
    - Zooming
 - Shader used for mouse picking
 - Render Interface
 - Render Tiles ([zig-zag approach](/docs/MECHANICS.md))
 - Render Buildings
 - Render Obstacles
 - Render Units
    - Different animations
 - Fog of war
 - High resolution support (!)

### Sound

 - Play sounds when selecting units
 - Play sounds when instructing units to move

### Music

 - WMA
 - MIDI

### Menus

 - Main menu
 - Loading screen
 - Custom map menu
 - Pause menu

### File Formats

 - Find campaign map data

### Future Enhancements

 - Easy military drag-select
 - Rally points
 - Configurable Gold multiplier
 - Multiplayer campaigns
 - Stable net play!
 - Random map generation
 - More endgame stats
 - Improved pathfinding
 - Non-suicidal peasants (don't go looking for gold)
 - Affordable Wall
 - Map preview / description
 - Using abilities with a group selected
 - Rank explanations
 - Replays
