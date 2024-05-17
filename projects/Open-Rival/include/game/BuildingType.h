#pragma once

#include <cstdint>

namespace Rival {

enum class BuildingType : std::uint8_t
{
    // Elf
    ElvenKeep,
    Treasury,
    CombatCamp,
    Arsenal,
    DuelRange,
    HolyNest,
    MinerGuildhall,
    AbbeyTower,
    CouncilOfRunes,
    Harbour,
    WarningTower,
    TreeWall,

    // Greenskin
    Fortress,
    HoardKeep,
    Fort,
    Blacksmith,
    BattleQuarters,
    BlackNest,
    WeirdWorkshop,
    UnholyChapel,
    AltarOfDoom,
    Docks,
    GuardTower,
    GreenskinWall,

    // Human
    Castle,
    GoldMill,
    ArcheryRange,
    Armoury,
    Barracks,
    HolyStables,
    FireGuild,
    Temple,
    MageTower,
    Shipyard,
    WatchTower,
    Wall,

    // Other
    Invalid,
    First = ElvenKeep,
    Last = Wall,
    ElfFirst = ElvenKeep,
    ElfLast = TreeWall,
    GreenskinFirst = Fortress,
    GreenskinLast = GreenskinWall,
    HumanFirst = Castle,
    HumanLast = Wall
};

}  // namespace Rival
