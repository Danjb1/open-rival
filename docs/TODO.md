# To Do

## General

 - Are all DLL files needed?

## Image Preparation

 - Images should conform to standard sizes (64x64, 128x128, etc.)
 - Where should sprites be positioned within their bounds?
    - They need to line up with other sprites in their animation
    - A consistent offset should be used to position sprites within tiles
    - (Theory) Images are already drawn at the right offset if images are kept to a standard size
 - Group images into textures:
    - Interface (menus, items, icons)
    - Tiles
    - Scenery (rocks, trees, chests)
    - Buildings - Human
    - Buildings - Elf
    - Buildings - Greenskin
    - Units - Human (should this be even more fine-grained?)
    - Units - Elf
    - Units - Greenskin
    - Units - Monsters
    - Effects (spells, bombs, traps?, cropland?)

## Engine

 - Create data structures:
    - Map
    - TileType
    - UnitType
    - BuildingType
    - ItemType
    - GameObject
        - Unit
        - Building
        - Obstacle
        - Door
 - Game loop

## Game Logic

 - Use the Command Pattern to manipulate game state?
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

## Input

 - Selecting units (render to a texture with mouse picking shader)
 - Drag-select

## Rendering

 - Shader used for mouse picking
 - Render Interface
 - Render Tiles ([zig-zag approach](/docs/MECHANICS.md))
 - Render Buildings
 - Render Obstacles
 - Render Units
    - Different orientations
    - Different animations
 - High resolution support (!)

## Sound

 - Play sounds when selecting units
 - Play sounds when instructing units to move

## Music

 - WMA
 - MIDI

## Menus

 - Main menu
 - Loading screen
 - Custom map menu
 - Pause menu

## File Formats

 - Decipher scenario format
 - Find campaign map data
