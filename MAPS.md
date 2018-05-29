# Scenario Format

The following analysis was performed with the help of VBinDiff.

## Notes

 - Data is saved in little-endian format unless otherwise specified.

 - If a scenario cannot be read, the editor will display an error: "Invalid scenario file!"

## Header

    8 bytes             Unknown (51 81 00 00 5B 81 00 00)
    1 byte              Terrain type (0x02 for meadows, 0x03 for wilderness)
    1 byte              Unknown (0x06)
    <=32 bytes          Map name
    1 byte              String terminator (0x28)
    1 byte              Map height
    3 bytes             (Empty)
    1 byte              Map width
    3 bytes             (Empty)

## Building Defaults

    Starting at offset 0x101E (for a map called "My map")

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

    Each building is 25 bytes:

        2 bytes         Hitpoints
        2 bytes         Armour
        1 byte          (Empty)
        1 byte          Sight
        1 byte          Range
        2 bytes         (Empty)
        1 byte          0xFF
        15 bytes        (Empty)

## Troop Defaults

TODO.

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

TODO.

## Buildings

TODO.

## Units

TODO.

### Facings

    00 = South
    01 = South-west
    02 = West
    03 = North-west
    04 = North
    05 = North-east
    06 = East
    07 = South-east

## Objects

TODO.

## Footer (end of file)

    1 byte              Checksum?

    Offset     Bytes                                                 Checksum
    0000 0000: 51 81 00 00 5B 81 00 00  02 06 4D 79 20 6D 61 70  =>  11
    0000 0000: 51 81 00 00 5B 81 00 00  02 06 4D 79 20 6D 62 70  =>  12
    0000 0000: 51 81 00 00 5B 81 00 00  03 06 4D 79 20 6D 61 70  =>  10
