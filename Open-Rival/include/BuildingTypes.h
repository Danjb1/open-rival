#ifndef BUILDING_TYPES_H
#define BUILDING_TYPES_H

namespace Rival {

    enum class BuildingType {

        // Elf
        ElvenKeep,      // HP: 1200, Armor: 12, Sight: 8, Range: 0
        Treasury,       // HP: 600, Armor: 4, Sight: 5, Range: 0
        CombatCamp,     // HP: 900, Armor: 8, Sight: 6, Range: 0
        Arsenal,        // HP: 900, Armor: 8, Sight: 6, Range: 0
        DuelRange,      // HP: 900, Armor: 8, Sight: 6, Range: 0
        HolyNest,       // HP: 900, Armor: 8, Sight: 6, Range: 0
        MinerGuildhall, // HP: 900, Armor: 8, Sight: 6, Range: 0
        AbbeyTower,     // HP: 900, Armor: 8, Sight: 6, Range: 0
        CouncilOfRunes, // HP: 900, Armor: 8, Sight: 6, Range: 0
        Harbour,        // HP: 900, Armor: 8, Sight: 6, Range: 0
        WarningTower,   // HP: 900, Armor: 12, Sight: 8, Range: 0
        TreeWall,       // HP: 600, Armor: 0, Sight: 4, Range: 0

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

    const auto firstElfBuildingType{ static_cast<int>(BuildingType::ElvenKeep) };
    const auto lastElfBuildingType{ static_cast<int>(BuildingType::TreeWall) };
    const auto firstGreenskinBuildingType{ static_cast<int>(BuildingType::Fortress) };
    const auto lastGreenskinBuildingType{ static_cast<int>(BuildingType::GreenskinWall) };
    const auto firstHumanBuildingType{ static_cast<int>(BuildingType::Castle) };
    const auto lastHumanBuildingType{ static_cast<int>(BuildingType::Wall) };

}

#endif // BUILDING_TYPES_H
