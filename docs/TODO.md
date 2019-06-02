# To Do

## Installer

 - Fully automate image extraction and texture composition
 - Automate sound extraction (file format unknown)
 - Script to extract all assets from game directory

## General

 - Set window icon
 - Log to file
 - Unit tests

## Image Preparation

 - Ensure images are always square!
 - When building textures, centre too-small images within their frame
 - Group remaining images into textures
 - Extract menu / interface images

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
 - Fog of war
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
