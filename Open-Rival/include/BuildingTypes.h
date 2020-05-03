#ifndef BUILDING_TYPES_H
#define BUILDING_TYPES_H

namespace Rival {

    enum class BuildingType {

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

    const auto firstElfBuildingType{ static_cast<int>(BuildingType::ElvenKeep) };
    const auto lastElfBuildingType{ static_cast<int>(BuildingType::TreeWall) };
    const auto firstGreenskinBuildingType{ static_cast<int>(BuildingType::Fortress) };
    const auto lastGreenskinBuildingType{ static_cast<int>(BuildingType::GreenskinWall) };
    const auto firstHumanBuildingType{ static_cast<int>(BuildingType::Castle) };
    const auto lastHumanBuildingType{ static_cast<int>(BuildingType::Wall) };

}

#endif // BUILDING_TYPES_H
