# To Do

## General

 - Are all DLL files needed?

## Image Preparation

 - Group images into textures:
    - Interface (menus, items, icons)
    - Tiles
    - Scenery (rocks, trees, chests)
    - Buildings - Human
    - Buildings - Elf
    - Buildings - Greenskin
    - Units (1 texture per unit)
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
