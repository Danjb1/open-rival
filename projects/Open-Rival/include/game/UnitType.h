#pragma once

#include <cstdint>  // uint8_t

namespace Rival {

enum class UnitType : std::uint8_t
{
    // Human
    Ballista,
    Battleship,
    Bowman,
    ChariotOfWar,
    FireMaster,
    Knight,
    LightCavalry,
    Peasant,
    PegasRider,
    Priest,
    SeaBarge,
    Thief,
    Wizard,
    Zeppelin,

    // Greenskin
    Balloon,
    Catapult,
    GnomeBoomer,
    HordeRider,
    LandingCraft,
    Necromancer,
    PriestOfDoom,
    RockThrower,
    Rogue,
    Serf,
    StormTrooper,
    TrollGalley,
    Warbat,
    Warlord,

    // Elf
    Archer,
    Arquebusier,
    Bark,
    Bombard,
    Centaur,
    Druid,
    DwarfMiner,
    Enchanter,
    Mage,
    MagicChopper,
    Scout,
    SkyRider,
    Warship,
    Yeoman,

    // Monsters
    Devil,
    Dragon,
    Golem,
    Gryphon,
    Hydra,
    SeaMonster,
    Skeleton,
    Snake,

    // Other
    Invalid,
    First = Ballista,
    Last = Snake
};

}  // namespace Rival
