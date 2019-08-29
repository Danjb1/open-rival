# To Do

## Improvements

 - Unit at (0, 0) is not visible
 - Static code analysis
 - Unit tests

## Features

### Installer

 - Fully automate image extraction and texture composition
 - Automate sound extraction
 - Script to extract all assets from game directory

### General

 - Set window icon
 - Log to file
 - Unit tests

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

 - Decipher scenario format
 - Find campaign map data
