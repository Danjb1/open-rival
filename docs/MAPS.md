# Scenario Format

The following analysis was performed with the help of VBinDiff.

## Notes

 - Data is saved in little-endian format unless otherwise specified.

 - Offsets are written assuming a default 64x40 map called "My map" with 2 units and start locations.

 - If a scenario cannot be read, the editor will display an error: "Invalid scenario file!"

## Header

    4 bytes             Unknown (?? 81 00 00) - offset?
    4 bytes             Unknown (?? 81 00 00) - offset?
    1 byte              Terrain type (0x02 = Meadows, 0x03 = Wilderness)
    1 byte              Unknown (0x06)
    <=32 bytes          Map name
    1 byte              String terminator (0x28)
    4 bytes             Map height
    4 bytes             Map width
    33 bytes            Unknown
    1 byte              Player 1 Race (0x02 = Human, 0x03 = Greenskin, 0x04 = Elf)

## Building Defaults

Starting at offset 0x101E.

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
    1 byte          (Empty)
    1 byte          Unknown (0x03)
    1 byte          Sight
    1 byte          Range
    2 bytes         (Empty)
    1 byte          Colour (see Appendices)
    36 bytes        (Empty)

## Monster Defaults

Starting at offset 0x1FEF.

Monsters are ordered by type (see Appendices).

Each monster consists of 47 bytes:

    2 bytes         Hitpoints
    2 bytes         Magic points
    2 bytes         Armor
    1 byte          Unknown (0x3E)
    1 byte          Sight
    1 byte          Range
    2 bytes         (Empty)
    1 byte          Colour (see Appendices)
    35 bytes        (Empty)

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

TODO.

## Unit Production Costs

TODO.

## Available Buildings ("Scenario Properties")

TODO.

## Hire Troops Restrictions

TODO.

## Scenario Goals

TODO.

## Alliances

TODO.

## Campaign Texts

TODO.

## Resource Info

TODO.

## AI Building Settings

TODO.

## AI Troops Settings

TODO.

## Player Properties

TODO.

## Terrain Data

Compressed?

## Buildings

Starting at offset 0x35FA:

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
    1 byte          Height? (0x00 for Land Units)
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
    1 byte          Player (00 = P1)

## Objects

TODO.

## Footer (end of file)

    16 bytes            Unknown (75 72 78 7C 84 54 34 F4 74 74 74 74 74 74 74 74)
    1 byte              Checksum?

    Offset     Bytes                                                 Checksum
    0000 0000: 51 81 00 00 5B 81 00 00  02 06 4D 79 20 6D 61 70  =>  11
    0000 0000: 51 81 00 00 5B 81 00 00  02 06 4D 79 20 6D 62 70  =>  12
    0000 0000: 51 81 00 00 5B 81 00 00  03 06 4D 79 20 6D 61 70  =>  10

## Appendices

### Buildings

    Human - Castle
    Human - Gold Mill
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
    0x3F    0x05        Human - Pegas Rider (Height = 0xA0)
    0x40    0x00        Human - Zeppelin (Height = 0x78)
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
    0x4D    0x1F        Greenskin - Warbat (Height = 0xB4)
    0x4E    0x00        Greenskin - Balloon (Height = 0x78)
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
    0x5B    0x31        Elf - Sky Rider (Height = 0xAF)
    0x5C    0x00        Elf - Magic Chopper (Height = 0x96)

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

 - Some typos in the weapon names in the Scenario Editor have been corrected here, e.g. White -> Cyan monsters.

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
