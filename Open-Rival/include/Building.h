#ifndef BUILDING_H
#define BUILDING_H

#include <cstdint>

namespace Rival { namespace Building {

    enum class Type : std::uint8_t
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
    };

    const int firstElfBuildingType = static_cast<int>(Type::ElvenKeep);
    const int lastElfBuildingType = static_cast<int>(Type::TreeWall);
    const int firstGreenskinBuildingType = static_cast<int>(Type::Fortress);
    const int lastGreenskinBuildingType = static_cast<int>(Type::GreenskinWall);
    const int firstHumanBuildingType = static_cast<int>(Type::Castle);
    const int lastHumanBuildingType = static_cast<int>(Type::Wall);

    static const int defaultWidth = 3;
    static const int defaultHeight = 2;
    static const int wallWidth = 1;
    static const int wallHeight = 1;

    int getWidth(Type type);
    int getHeight(Type type);
    bool isWall(Type type);

}}  // namespace Rival::Building

#endif  // BUILDING_H
