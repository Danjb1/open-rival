# To Do

<!----------------------------------------------------------------------------->
## Bugs
<!----------------------------------------------------------------------------->

- Ranged units sometimes keep attacking after the target dies
- Trying to attack an empty tile with a ranged unit just moves there instead
- Ground units should not move when commanded to attack a flying unit
- Flying unit hitboxes should take precedence over ground units behind them
- Flying units clear passability when leaving a tile even if it still occupied by a ground unit
- Hitboxes should not be rendered in Attack mode

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

### Combat

- Units should automatically target nearby enemies (needs spatial partitioning!)
    - Respect "Fighting Area" - do not wander too far
- Units should rest when idle (approx. 200 health in 30 seconds)
- Allow attacking buildings
- Allow attacking an empty tile when in attack mode (ranged attacks only!)
    - For melee attacks it should display a message "Could not attack there !"
- Destroying cropland
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
