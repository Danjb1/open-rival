# Scenario Format

> This information may be outdated.
>
> The definitive format can be deduced from the [Scenario Reader](/Open-Rival/src/ScenarioReader.cpp).

The following analysis was performed with the help of VBinDiff.

## Notes

 - Data is saved in little-endian format unless otherwise specified.

 - Offsets are written assuming a default 64x40 map called "My map" with 2 units and start locations.

 - If a scenario cannot be read (or if the LRC is invalid), the editor will display an error: "Invalid scenario file!"

## Section Ordering

    Offset      Section
    ------------------------------------------
    0x0000      Header
    0x0019      Player Properties
    0x001E      Building Defaults
    0x0496      Troop Defaults
    0x0C7D      Upgrade Properties
    0x142D      Unit Production Costs
    0x17FC      Weapon Defaults
    0x1FDE      Available Buildings ("Scenario Properties")
    0x1FEF      Monster Defaults
    0x25C0      Hire Troops Restrictions
    0x25CF      AI Building Settings
    0x2677      AI Troops Settings
    0x28C3      Terrain Data
    0x80DD      Units
    0x814D      Traps
    0x8178      Scenario Goals
    0x817B      Campaign Texts
    ???         Buildings
    ???         Objects

## Header

    4 bytes             Unknown (?? 81 00 00) - end of terrain offset?
    4 bytes             Unknown (?? 81 00 00) - footer offset?
    1 byte              Terrain type (0x02 = Meadows, 0x03 = Wilderness)
    1 byte              Map name length
    <=32 bytes          Map name
    4 bytes             Map height
    4 bytes             Map width
    5 bytes             00 00 00 00 00 00 or 35 02 00 00 48 02 (seems to change arbitrarily)

## Building Defaults

Starting at offset 0x001E?

Buildings are ordered by type (see Appendices).

Each building consists of 25 bytes:

    2 bytes         Hitpoints
    2 bytes         Armour
    1 byte          (Empty)
    1 byte          Sight
    1 byte          Range
    2 bytes         (Empty)
    1 byte          Colour (see Appendices)
    15 bytes        (Empty)

## Troop Defaults

Starting at offset 0x0496.

Units are ordered by type (see Appendices).

Each unit consists of 47 bytes:

    2 bytes         Hitpoints
    2 bytes         Magic points
    1 byte          Armour
    2 bytes         Unknown
    1 byte          Sight
    1 byte          Range
    2 bytes         (Empty)
    1 byte          Colour (see Appendices)
    36 bytes        (Empty)

## Monster Defaults

Starting at offset 0x1FEF.

Monsters are ordered by type (see Appendices). Their format is the same as Troop Defaults.

## Weapon Defaults

Starting at offset 0x17FC.

Weapons are ordered by type (see Appendices).

Each weapon consists of 21 bytes:

    2 bytes         Move space
    2 bytes         Move time
    2 bytes         Damage
    2 bytes         Penetrate
    2 bytes         Accuracy
    1 byte          Effect range
    1 byte          Attack range
    2 bytes         Mana cost
    4 bytes         Reload time (ms)
    1 byte          (Empty)
    1 byte          Unknown (0x7E)
    1 byte          (Empty)

## Upgrade Properties

Starting at offset 0x0C7D.

Upgrades are ordered by type (see Appendices).

Each upgrade consists of 16 bytes:

    4 bytes         Amount*
    4 bytes         Gold Cost
    4 bytes         Wood Cost
    4 bytes         Unknown (icon / type?)

Amount is only applicable to the first 48 Upgrades, plus the 3 thief "Swiftness" upgrades (list indices 0-47 and 67-69).

## Unit Production Costs

Starting at offset 0x142D.

First buildings are listed, then units, ordered by type (see Appendices) - and then Cropland!

Each item consists of 12 bytes:

    2 bytes         Gold cost
    2 bytes         Wood cost
    4 bytes         Time of construction
    4 bytes         Experience need (buildings) / increase percent (units)

## Available Buildings ("Scenario Properties")

Starting at offset 0x1FDE.

Each entry consists of 1 byte (0x00 = Disabled, 0x01 = Enabled), in the order listed:

    Crop Land
    Gold Mill / Treasury / Hoard Keep
    Archery Range / Combat Camp / Fort
    Armoury / Arsenal / Blacksmith
    Barracks / Duel Range / Battle Quarters
    Holy Stables / Holy Nest / Black Nest
    Fire Guild / Miner Guildhall / Weird Workshop
    Temple / Abbey Tower / Unholy Cathedral
    Mage Tower / Council of Runes / Altar of Doom
    Shipyard / Harbour / Docks
    Watch Tower / Warning Tower / Guard Tower
    Human Wall / Tree Wall / Greenskin Wall

## Hire Troops Restrictions

Starting at offset 0x25C0.

Each entry consists of 1 byte (0x00 = Disabled, 0x01 = Enabled).

There is an entry for all Human units, ordered by unit type (see Appendices). Presumably the same entries are used for other races, but only Human units are listed in the Scenario Editor.

After all unit types is another byte for "Must Hire".

## Scenario Goals

Starting at offset 0x8178 (immediately before the footer) - generally preceded by 0x34 0xF4.

This section uses a strange numbering system, described in *goals_numbering.xlsx*.

0x74 is represented by a dash, below.

0xAF also seems to have some special meaning, represented by an underscore, below.

The section is formatted as follows:

    1 byte(?)       Number of goals
    ? bytes         List of goals (each goal type is described below)
    6 bytes         0x74 0x74 0x74 0x74 0x74 0x74

### BUILD

       ??    ??
    -- TT -- NN __ __ -- __ __ __ __ -- 24 A2 8D

 - `TT` is the building type (see Appendices)

 - `NN` is the count

### BUILD AT

    ?? ??    ??          ??          ??
    RR TT -- NN __ __ -- XX -- -- -- YY -- -- --

 - `RR` is the radius

 - `TT` is the building type (see Appendices)

 - `NN` is the count

 - `XX` is the x co-ordinate

 - `YY` is the y co-ordinate

### DESTROY

       ??    ??
    72 TT -- PP __ __ -- __ __ __ __ -- 24 A2 8D

 - `TT` is the building type (see Appendices)

 - `PP` is the player number

### TOUCH

       ??          ??    ??          ??
    73 TT -- __ __ ID -- XX -- -- -- YY -- -- --

 - `TT` is the unit type (ordering same as in the appendices, but starting from 0x63)

 - `ID` is the Troop ID

 - `XX` is the x co-ordinate

 - `YY` is the y co-ordinate

### TOUCH ITEM

                ??       ??          ??
    78 __ __ __ II __ -- XX -- -- -- YY -- -- --

 - `II` is the item type

 - `XX` is the x co-ordinate

 - `YY` is the y co-ordinate

### GET

       TT       II ID
    79 63 -- __ 75 75 -- __ __ __ __ -- 24 A2 8D

 - `TT` is the unit type

 - `ID` is the Troop ID

 - `II` is the item type

### ELIMINATE

    76 __ __ __ __ __ -- __ __ __ __ -- 24 A2 8D

## Alliances

Starting at offset 0x815F (almost at the end).

1 byte per player. Values seem arbitrary but players with the same value are allied!

Default:

    75 72 78 7C 84 54 34 F4

Players allied in pairs:

    73 73 80 80 64 64 B4 B4

## Campaign Texts

Starting at offset 0x817B (immediately before the checksum).

Uses character values from *[alphabet.md](/docs/alphabet.md)*

If the first byte is 0x74, then there is no campaign text, and the subsequent values are simply `74 74 74 74 74`.

    73 74 35 32 33 73 74 18 19 16 73 74 1C 1D 1A
           A  B  C        d  e  f        h  i  j

    Title: ABC
    Objectives: abc
    Narration: hij

## AI Building Settings

For each AI strategy (see Appendices), the following buildings are listed:

    Gold Mill / Treasury / Hoard Keep
    Archery Range / Combat Camp / Fort
    Armoury / Arsenal / Blacksmith
    Barracks / Duel Range / Battle Quarters
    Holy Stables / Holy Nest / Black Nest
    Fire Guild / Miner Guildhall / Weird Workshop
    Temple / Abbey Tower / Unholy Cathedral
    Mage Tower / Council of Runes / Altar of Doom
    Shipyard / Harbour / Docks
    Watch Tower / Warning Tower / Guard Tower
    Human Wall / Tree Wall / Greenskin Wall

Each entry contains:

    1 byte          Amount
    1 byte          Flag

No idea what "Flag" does, but it's always 1 or 2. Could be something to do with which Upgrades to buy.

## AI Troops Settings

For each AI strategy (see Appendices), the following Units are listed:

    Peasant / Yeoman / Serf
    Bowman / Archer / Rock Thrower
    Light Cavalry / Druid / Horde Rider
    Knight / Centaur / Warlord
    Fire Master / Dwarf Miner / Gnome Boomer
    Thief / Scout / Rogue
    Ballista / Bombard / Catapult
    Chariot of War / Arquebusier / Storm Trooper
    Wizard / Mage / Priest of Doom
    Priest / Enchanter / Necromancer
    Sea Barge / Bark / Landing Craft
    Battleship / Warship / Troll Galley
    Pegas Rider / Sky Rider / Warbat
    Zeppelin / Magic Chopper / Balloon

Each entry contains:

    1 byte          Amount
    1 byte          X

No idea what "X" does, but it's only non-zero for Fire Masters, Thieves and Zeppelins. Could be something to do with which Upgrades to buy.

## Player Properties

Starting at offset 0x0019 (immediately after the header).

Repeats for each player.

    12 bytes        Unknown
    4 bytes         Starting Food
    4 bytes         Starting Wood
    4 bytes         Starting Gold
    1 byte          Race (0x02 = Human, 0x03 = Greenskin, 0x04 = Elf)
    1 byte          Human (0x00) or Computer (0x01)
    1 byte          AI Type (0x00 = Passive, 0x01 = Offensive, 0x02 = Defensive)
    1 byte          AI Performance (see Appendices)
    1 byte          AI Strategy (see Appendices)

## Terrain Data

Tiles are stored row by row using a kind of run-length encoding.

A tile that is different to the one that follows it will take the form:

    FF FF FF FF [tile]

But when multiple consecutive tiles are the same, they will be described as:

    FA NN NN FF FF FF FF [tile]

where `NN NN` is the number of tiles that this describes.

Each tile type has a number of different representations, possibly corresponding to graphical variants. Samples are given below, where 0x00 is represented by two dashes.

### Grass

    -- -- -- -- -- --
    -- -- -- -- 02 --
    -- -- -- -- -- --
    -- -- -- -- 05 --
    -- -- -- -- 04 --
    -- -- -- -- 01 --
    -- -- -- -- 03 --
    -- -- -- -- 02 --
    -- -- -- -- 01 --
    -- -- -- -- 03 --
    -- -- -- -- 01 --
    -- -- -- -- 04 --
    -- -- -- -- 01 --
    -- -- -- -- 04 --
    -- -- -- -- -- --
    -- -- -- -- 03 --
    -- -- -- -- -- --
    -- -- -- -- 01 --
    -- -- -- -- 05 --

### Water

    -- -- 06 -- 01 --
    -- -- 0F -- 04 --
    -- -- 0F -- 03 --
    -- -- 0F -- 04 --
    -- -- 07 -- 01 --
    -- -- 0F -- 03 --
    -- -- 07 -- 01 --
    -- -- 0F -- 01 --
    -- -- 0F -- 03 --
    -- -- 0F -- 02 --
    -- -- 0F -- -- --
    -- -- 0F -- 04 --
    -- -- 0F -- 03 --
    -- -- 0F -- 04 --
    -- -- 0F -- 01 --
    -- -- 07 -- -- --
    -- -- 0F -- 02 --
    -- -- 0F -- -- --
    -- -- 0F -- 04 -- 

### Mud

    -- -- 1E -- 02 --
    -- -- 1E -- -- --
    -- -- 1E -- 01 --
    -- -- 1E -- 04 --
    -- -- 1E -- 01 --
    -- -- 1E -- -- --
    -- -- 1E -- 01 --
    -- -- 1E -- 02 --
    -- -- 1E -- 01 --
    -- -- 1E -- 04 --
    -- -- 1E -- 01 --
    -- -- 1E -- -- --
    -- -- 1E -- 03 --
    -- -- 1E -- 02 --
    -- -- 1E -- -- --
    -- -- 1E -- 02 --
    -- -- 1E -- 03 --
    -- -- 1E -- 04 --
    -- -- 1E -- 01 --
    -- -- 1E -- 03 --

### Dirt

    -- -- 2D -- 04 -- 
    -- -- 2D -- 01 -- 
    -- -- 2D -- 02 -- 
    -- -- 2D -- 04 -- 
    -- -- 2D -- 01 -- 
    -- -- 2D -- 04 -- 
    -- -- 2D -- -- -- 
    -- -- 2D -- 04 -- 
    -- -- 2D -- 02 -- 
    -- -- 2D -- 04 -- 
    -- -- 2D -- 01 --
    -- -- 2D -- 03 -- 
    -- -- 2D -- 04 -- 
    -- -- 2D -- -- -- 
    -- -- 2D -- 01 --
    -- -- 2D -- 02 --
    -- -- 2D -- 04 -- 
    -- -- 2D -- -- -- 
    -- -- 2D -- 03 --

### Dungeon

    -- -- 3C -- 01 --
    -- -- 3C -- -- --
    -- -- 3C -- 03 --
    -- -- 3C -- 05 --
    -- -- 3C -- -- --
    -- -- 3C -- 05 --
    -- -- 3C -- 03 --
    -- -- 3C -- 04 --
    -- -- 3C -- 01 --
    -- -- 3C -- -- --
    -- -- 3C -- 02 --
    -- -- 3C -- -- --
    -- -- 3C -- 04 --
    -- -- 3C -- 01 --
    -- -- 3C -- -- --
    -- -- 3C -- 02 --
    -- -- 3C -- 01 --
    -- -- 3C -- 03 --
    -- -- 3C -- 0C --
    -- -- 3C -- 04 -- 

### Wood

    -- -- -- -- 04 --
    -- -- -- -- 05 --
    -- -- -- -- -- --
    -- -- -- -- 03 --
    -- -- -- -- -- --
    -- -- -- -- 04 --
    -- -- -- -- 03 --
    -- -- -- -- 02 --
    -- -- -- -- 03 --
    -- -- -- -- 01 --
    -- -- -- -- 03 --
    -- -- -- -- 04 --
    -- -- -- -- 02 --
    -- -- -- -- 05 --
    -- -- -- -- -- --
    -- -- -- -- 05 --
    -- -- -- -- -- --
    -- -- -- -- 01 --
    -- -- -- -- 01 03
    -- -- -- -- -- 03

### Gold

    01 -- -- 02 -- 01
    01 -- -- 01 -- 01
    01 -- -- 02 -- 01
    01 -- -- -- -- 01
    01 -- -- 02 -- 01
    01 -- -- 03 -- 01
    01 -- -- 02 -- 01
    01 -- -- 01 -- 01
    01 -- -- 02 -- 01
    01 -- -- -- -- 01
    01 -- -- 02 -- 01
    01 -- -- 01 -- 01
    01 -- -- -- -- 01
    01 -- -- 02 -- 01
    01 -- -- 03 -- 01
    01 -- -- 01 -- 01
    01 -- -- 02 -- 01
    01 -- -- 03 -- 01
    01 -- -- -- -- 01

### Cropland

    02 -- -- -- -- 01

## Buildings

Mingled with terrain data?

    1 byte          Buildings present (0x03 = False, 0x04 = True)

Each building on the map consists of 23 bytes:

    2 bytes         Hitpoints
    2 bytes         Armour
    1 byte          (Empty)
    1 byte          Sight
    1 byte          Range
    1 byte          Upgrade 1 enabled (0x00 or 0x01)
    1 byte          Upgrade 2 enabled (0x00 or 0x01)
    1 byte          Special colour (see Appendices)
    1 byte          Prisoner (0x00 or 0x01)
    12 bytes        Name

## Units

Starting at offset 0x80DD.

    4 bytes         Number of units

Each unit on the map consists of 57 bytes:

    1 byte          Type (see Appendices)
    2 bytes         (Empty)
    1 byte          Facing (see Appendices)
    1 byte          (Empty)
    2 bytes         X
    2 bytes         Y
    1 byte          Player (00 = P1, 01 = P2, etc.)
    2 bytes         Hitpoints
    1 byte          Magic points
    2 bytes         Armour
    1 byte          (Empty)
    1 byte          Type2 (see Appendices)
    1 byte          Sight
    1 byte          Range
    2 bytes         (Empty)
    1 byte          Special colour (see Appendices)
    1 byte          Prisoner (0x00 or 0x01)
    2 bytes         Gold cost
    2 bytes         Wood cost
    12 bytes        Name
    13 bytes        (Empty)
    1 byte          Upgrade 1 enabled (0x00 or 0x01)
    1 byte          Upgrade 2 enabled (0x00 or 0x01)
    1 byte          Upgrade 3 enabled (0x00 or 0x01)
    1 byte          Upgrade 4 enabled (0x00 or 0x01)
    1 byte          Fighting area (0x00 = No Limit, 0x01 = Stand Ground, 0x02 = Range 1, etc.)

 - Adding Pegas Riders (and maybe other flying units) seems to change some bytes:
    - Add for 1 Player:  0x2D30 (03 -> 0B)
    - Add for 2 Players: 0x31B4 (03 -> 13)

## Traps

Starting at offset 0x814D.

    4 bytes         Number of traps
    1 byte          X
    1 byte          Y
    1 byte          Player (00 = P1, 01 = P2, etc.)

## Objects

Starting almost immediately after terrain data.

Section starts with the number of objects (4 bytes).

### Trees

    03 00 VV XX 00 00 00 YY 00 00 00

 - `VV` represents the graphical variant (?)

 - `XX` is the x co-ordinate of the tree

 - `YY` is the y co-ordinate of the tree

## Footer (end of file)

    1 byte              LRC

## Appendices

### Buildings

 - Type2 is what the Scenario Goals section uses, which uses a strange numbering system.

<!-- End of bullet list -->

    Type    Type2   Building
    ------------------------------------------
    0x0F    0x7F    Human - Castle
    0x10    0x84    Human - Gold Mill
                    Human - Archery Range
                    Human - Armoury
                    Human - Barracks
                    Human - Holy Stables
                    Human - Fire Guild
                    Human - Temple
                    Human - Mage Tower
                    Human - Shipyard
                    Human - Watch Tower
                    Human - Wall
                    Greenskin - Fortress
                    Greenskin - Hoard Keep
                    Greenskin - Fort
                    Greenskin - Blacksmith
                    Greenskin - Battle Quarters
                    Greenskin - Black Nest
                    Greenskin - Weird Workshop
                    Greenskin - Unholy Chapel
                    Greenskin - Altar of Doom
                    Greenskin - Docks
                    Greenskin - Guard Tower
                    Greenskin - Greenskin Wall
                    Elf - Elven Keep
                    Elf - Treasury
                    Elf - Combat Camp
                    Elf - Arsenal
                    Elf - Duel Range
                    Elf - Holy Nest
                    Elf - Miner Guildhall
                    Elf - Abbey Tower
                    Elf - Council of Runes
                    Elf - Harbour
                    Elf - Warning Tower
                    Elf - Tree Wall

### Units

    Type    Type2       Unit
    ------------------------------------------
    0x33    0x03        Human - Peasant
    0x34    0x04        Human - Bowman
    0x35    0x07        Human - Light Cavalry
    0x36    0x08        Human - Knight
    0x37    0x0A        Human - Fire Master
    0x38    0x0B        Human - Thief
    0x39    0x0C        Human - Ballista
    0x3A    0x00        Human - Chariot of War
    0x3B    0x0E        Human - Wizard
    0x3C    0x12        Human - Priest
                        Human - Sea Barge
                        Human - Battleship
    0x3F    0x05        Human - Pegas Rider
    0x40    0x00        Human - Zeppelin
    0x41    0x03        Greenskin - Serf
    0x42    0x16        Greenskin - Rock Thrower
    0x43    0x17        Greenskin - Horde Rider
    0x44    0x18        Greenskin - Warlord
    0x45    0x1A        Greenskin - Gnome Boomer
    0x46    0x1B        Greenskin - Rogue
    0x47    0x1D        Greenskin - Catapult
    0x48    0x1C        Greenskin - Storm Trooper
    0x49    0x20        Greenskin - Priest of Doom
    0x4A    0x24        Greenskin - Necromancer
                        Greenskin - Landing Craft
                        Greenskin - Troll Galley
    0x4D    0x1F        Greenskin - Warbat
    0x4E    0x00        Greenskin - Balloon
    0x4F    0x03        Elf - Yeoman
    0x50    0x28        Elf - Archer
    0x51    0x33        Elf - Druid
    0x52    0x2A        Elf - Centaur
    0x53    0x2C        Elf - Dwarf Miner
    0x54    0x2D        Elf - Scout
    0x55    0x2E        Elf - Bombard
    0x56    0x2F        Elf - Arquebusier
    0x57    0x37        Elf - Mage
    0x58    0x3A        Elf - Enchanter
                        Elf - Bark
                        Elf - Warship
    0x5B    0x31        Elf - Sky Rider
    0x5C    0x00        Elf - Magic Chopper

### Monsters

    Black Sea Monster
    Blue Sea Monster
    Green Sea Monster
    Black Snake
    Red Snake
    Green Snake
    Yellow Snake
    Black Gryphon
    Yellow Gryphon
    Cyan Gryphon
    Black Hydra
    Blue Hydra
    Green Hydra
    Black Golem
    Blue Golem
    Cyan Golem
    Black Devil
    Red Devil
    Orange Devil
    Yellow Devil
    Black Skeleton
    Red Skeleton
    Cyan Skeleton
    Yellow Skeleton
    Black Dragon
    Red Dragon
    Violet Dragon
    Blue Dragon
    Orange Dragon
    Green Dragon

### Weapons

 - Some typos present in the Scenario Editor have been corrected here, e.g. White -> Cyan monsters.

<!-- End of bullet list -->

    Bomb
    Trap
    Axe - Workers
    Arrow - Archers
    One Arrow - Pegas
    Three Arrows - Pegas
    Light Sword - Cavalry
    Sword - Knight
    Mace - Knight
    Dagger - Fire Master
    Dagger - Thief
    Big Arrow - Ballista
    Projectile - Battleship
    Flash Light - Wizard
    Fire Ball - Wizard
    Frozen Breath - Wizard
    Cursed Rain - Wizard
    Flash Light - Priest
    Dispel Mana - Priest
    Reveal Traps - Priest
    Poison Cropland - Priest
    Stone - Rock Thrower
    Spear - Horde Rider
    Battle Axe - Warlord
    Poison Spit - Warlord
    Dagger - Gnome Boomer
    Dagger - Rogue
    War Stick - Storm Trooper
    Giant Rock - Catapult
    Boulder - Troll Galley
    Flames - War Bat
    Flash Light - Priest of Doom
    Death Typhoon - Priest of Doom
    Hyena Howl - Priest of Doom
    Poison Breath - Priest of Doom
    Flash Light - Necromancer
    Drain Life - Necromancer
    Reveal Traps - Necromancer
    Cursed Land - Necromancer
    Arrow - Archer
    Fire Arrow - Archer
    Halberd - Centaur
    Flame Ring - Centaur
    Dagger - Dwarf Miner
    Dagger - Scout
    Cannon Ball - Bombard
    Shell - Arquebusier
    Projectile - Warship
    Flash Light - Sky Rider
    Lightning - Sky Rider
    Flash Light - Mage
    Touch of Chaos - Mage
    Magic Barrier - Mage
    Blindness - Mage
    Flash Light - Druid
    Berserker - Druid
    Lightning Bolt - Druid
    Flash Light - Enchanter
    Reveal Traps - Enchanter
    Healing - Enchanter
    Light Defence Arrow
    Black Sea Monster weapon
    Blue Sea Monster weapon
    Green Sea Monster weapon
    Black Snake weapon
    Red Snake weapon
    Green Snake weapon
    Yellow Snake weapon
    Black Gryphon weapon
    Yellow Gryphon weapon
    White Gryphon weapon
    Black Hydra weapon
    Blue Hydra weapon
    Green Hydra weapon
    Black Golem weapon
    Blue Golem weapon
    Cyan Golem weapon
    Black Devil weapon
    Red Devil weapon
    Orange Devil weapon
    Yellow Devil weapon
    Black Skeleton weapon
    Red Skeleton weapon
    Cyan Skeleton weapon
    Yellow Skeleton weapon
    Black Dragon weapon
    Red Dragon weapon
    Violet Dragon weapon
    Blue Dragon weapon
    Orange Dragon weapon
    Green Dragon weapon
    Key-Door Destroyer
    Soul Damnation
    Spyral Scroll
    Wave Scroll
    Teleport Scroll

### Upgrades

 - Some typos present in the Scenario Editor have been corrected here.

<!-- End of bullet list -->

    Armor - Knight
    Armor - Chariot of War
    Armor - Sea Barge
    Armor - Battleship
    Armor - Zeppelin
    Armor - Warlord
    Armor - Storm Trooper
    Armor - Landing Craft
    Armor - Troll Galley
    Armor - Balloon
    Armor - Centaur
    Armor - Bark
    Armor - Warship
    Armor - Magic Chopper
    Damage - Light Cavalry
    Damage - Knight
    Damage - Ballista
    Damage - Battleship
    Damage - Warlord
    Damage - Storm Trooper
    Damage - Troll Galley
    Damage - Warship
    Speed - Light Cavalry
    Speed - Chariot of War
    Speed - Horde Rider
    Accuracy - Bowman
    Accuracy - Knight
    Accuracy - Ballista
    Accuracy - Battleship
    Accuracy - Rock Thrower
    Accuracy - Catapult
    Accuracy - Troll Galley
    Accuracy - Archer
    Accuracy - Centaur
    Accuracy - Bombard
    Accuracy - Arquebusier
    Accuracy - Warship
    Latency - Bowman
    Latency - Battleship
    Latency - Rock Thrower
    Latency - Troll Galley
    Latency - Archer
    Latency - Warship
    Sight - Bowman
    Sight - Rock Thrower
    Range - Bowman
    Range - Pegas Rider
    Range - Archer
    Knight - Mace
    Pegas Rider - Burst 3 Arrows
    Warlord - Poison Spit
    Warbat - Capture
    Centaur - Flame Ring
    Archer - Fire Arrow
    Sky Rider - Lightning
    Human - Create Bomb
    Human - Create Trap
    Human - Land / Water
    Human - Repair
    Greenskin - Create Bomb
    Greenskin - Create Trap
    Greenskin - Land / Water
    Greenskin - Repair
    Elf - Create Bomb
    Elf - Create Trap
    Elf - Land / Water
    Elf - Repair
    Human Thief - Swiftness
    Greenskin Rogue - Swiftness
    Elf Scout - Swiftness
    Human Thief - Invisibility
    Greenskin Rogue - Invisibility
    Elf Scout - Invisibility
    Wizard - Magic Shield
    Wizard - Fire Ball
    Wizard - Frozen Breath
    Wizard - Cursed Rain
    Priest - Magic Shield
    Priest - Dispel Mana
    Priest - Reveal Traps
    Priest - Poison Cropland
    Priest of Doom - Magic Shield
    Priest of Doom - Death Typhoon
    Priest of Doom - Hyena Howl
    Priest of Doom - Poisonous Breath
    Necromancer - Magic Shield
    Necromancer - Drain Life
    Necromancer - Reveal Traps
    Necromancer - Cursed Land
    Mage - Magic Shield
    Mage - Magic Barrier
    Mage - Blindness
    Mage - Touch of Chaos
    Druid - Magic Shield
    Druid - Teleport
    Druid - Berserk
    Druid - Lightning Bolt
    Enchanter - Magic Shield
    Enchanter - Teleport
    Enchanter - Reveal Traps
    Enchanter - Healing
    Human - Train 3 Units
    Greenskin - Train 3 Units
    Elf - Train 3 Units
    Human - Increase Wood Production
    Greenskin - Increase Wood Production
    Elf - Increase Wood Production
    Human Tower - Watch
    Greenskin Tower - Watch
    Elf Tower - Watch
    Human - Defence
    Greenskin - Defence
    Elf - Defence

### Items

    Index   Item
    ------------------------------------------
    0       Gold
    1       Money
    2       Gold Bonus
    3       Wood
    4       Wood Bonus
    5       Normal Food
    6       Poisonous Food
    7       Food Bonus
    8       Healing Potion
    9       Healing Bonus
    10      Mana Potion
    11      Mana Bonus
    12      Bomb
    13      Ring of Protection
    14      Bracelet of Defender
    15      Crown of Titans
    16      Helm of Dragons
    17      Ring of Power
    18      Bracelet of Strength
    19      Gauntlet of Might
    20      Amulet of Battle
    21      Crown of Destruction
    22      Necklace of Dragons
    23      Ring of Skill
    24      Celestial Cloak
    25      Tome of Knowledge
    26      Magical Efficiency Sphere
    27      Supreme Magic Shield
    28      Boots of Windspeed
    29      Ring of Fury
    30      Bracelet of Fanaticism
    31      Ring of Life
    32      Necklace of Blessing
    33      Amulet of Life
    34      Healing Scroll
    35      Spyral Scroll
    36      Wave Scroll
    37      Soul Damnation Scroll
    38      Frozen Breath Scroll
    39      Hyena Howl Scroll
    40      Touch of Chaos Scroll
    41      Teleport Scroll
    42      Drain Life Scroll
    43      Magic Barrier Scroll
    44      Curse Land Scroll
    45      Poison Cropland Scroll
    46      Berserker Scroll
    47      Blindness Scroll
    48      Red Key
    49      Green Key
    50      Blue Key
    51      Yellow Key

### Facings

    0x00 = South
    0x01 = South-west
    0x02 = West
    0x03 = North-west
    0x04 = North
    0x05 = North-east
    0x06 = East
    0x07 = South-east

### Colours

    0xFF = Default
    0x00 = Red
    0x01 = Cyan
    0x02 = Green
    0x03 = Yellow
    0x04 = Orange
    0x05 = Violet
    0x06 = Blue
    0x07 = Black

### AI Strategies

    0x01 = Land Combat
    0x02 = General
    0x03 = Sea Wolf
    0x04 = Wizards
    0x05 = Airbourne
    0x06 = Custom 1
    0x07 = Custom 2

### AI Performance

    No harrassment:

    0x10 = Have mercy
    0x11 = I am OK
    0x12 = I will chop you
    0x13 = Try This!

    With harrassment:

    0x00 = Have mercy
    0x01 = I am OK
    0x02 = I will chop you
    0x03 = Try This!
