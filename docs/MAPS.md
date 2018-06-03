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

Buildings:

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

Each building consists of 25 bytes:

    2 bytes         Hitpoints
    2 bytes         Armour
    1 byte          (Empty)
    1 byte          Sight
    1 byte          Range
    2 bytes         (Empty)
    1 byte          0xFF
    15 bytes        (Empty)

## Troop Defaults

Starting at offset 0x0496.

Unit are ordered by type (see below).

Each unit consists of 8 bytes:

    2 bytes         Hitpoints
    2 bytes         Magic points
    1 byte          (Empty)
    1 byte          Unknown (0x03)
    1 byte          Sight
    1 byte          Range

## Monster Defaults

TODO.

## Weapon Defaults

TODO.

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
    1 byte          Special colour (0xFF = Default, 0x00 = Red, 0x01 = Cyan, etc.)
    1 byte          Prisoner (0x00 or 0x01)
    12 bytes        Name

## Units

Starting at offset 0x80DD.

    4 bytes         Number of units

Each unit on the map consists of 57 bytes:

    1 byte          Type
    2 bytes         (Empty)
    1 byte          Facing (0x00 = South, 0x01 = South-west, etc.)
    1 byte          (Empty)
    2 bytes         X
    2 bytes         Y
    1 byte          Height? (0x00 for Land Units)
    2 bytes         Hitpoints
    1 byte          Magic points
    2 bytes         Armour
    1 byte          (Empty)
    1 bytes         Type2 (not race specific - see table below)
    1 byte          Sight
    1 byte          Range
    2 bytes         (Empty)
    1 byte          Special colour (0xFF = Default, 0x00 = Red, 0x01 = Cyan, etc.)
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

### Unit Type

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
