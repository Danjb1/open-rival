#ifndef BUILDING_ANIMATION_LOOKUP_H
#define BUILDING_ANIMATION_LOOKUP_H

#include <map>

#include "BuildingTypes.h"

namespace Rival {

    // This needs a better naming, please correct
    // We have 16 wall variants. This enum class, when considered as an int,
    //     corresponds to wall sprite offset in SpriteSheet
    enum class WallVariant {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P
    };

    enum class BuildingAnimationType {
        Constructing,
        Built
    };

    // Spritesheet entry start index and end index for
    //   each pair of BuildingAnimationType
    using BuildingAnimationPair = std::pair<BuildingType, BuildingAnimationType>;
    using SpritesheetEntry = std::pair<int, int>;
    using BuildingAnimationLookup = std::map<BuildingAnimationPair, SpritesheetEntry>;
    static const BuildingAnimationLookup buildingAnimationLookup = {

        // Mappings for Elf buildings
        { {BuildingType::ElvenKeep, BuildingAnimationType::Constructing}, { 0, 1 } },
        { {BuildingType::ElvenKeep, BuildingAnimationType::Built}, { 2, 2 } },
        { {BuildingType::Treasury, BuildingAnimationType::Constructing}, { 3, 4 } },
        { {BuildingType::Treasury, BuildingAnimationType::Built}, { 5, 11 } },
        { {BuildingType::CombatCamp, BuildingAnimationType::Constructing}, { 12, 13 } },
        { {BuildingType::CombatCamp, BuildingAnimationType::Built}, { 14, 14 } },
        { {BuildingType::Arsenal, BuildingAnimationType::Constructing}, { 15, 16 } },
        { {BuildingType::Arsenal, BuildingAnimationType::Built}, { 17, 17 } },
        { {BuildingType::DuelRange, BuildingAnimationType::Constructing}, { 18, 19 } },
        { {BuildingType::DuelRange, BuildingAnimationType::Built}, { 20, 20 } },
        { {BuildingType::HolyNest, BuildingAnimationType::Constructing}, { 21, 22 } },
        { {BuildingType::HolyNest, BuildingAnimationType::Built}, { 23, 23 } },
        { {BuildingType::MinerGuildhall, BuildingAnimationType::Constructing}, { 24, 25 } },
        { {BuildingType::MinerGuildhall, BuildingAnimationType::Built}, { 26, 26 } },
        { {BuildingType::AbbeyTower, BuildingAnimationType::Constructing}, { 27, 28 } },
        { {BuildingType::AbbeyTower, BuildingAnimationType::Built}, { 29, 29 } },
        { {BuildingType::CouncilOfRunes, BuildingAnimationType::Constructing}, { 30, 31 } },
        { {BuildingType::CouncilOfRunes, BuildingAnimationType::Built}, { 32, 41 } },
        { {BuildingType::Harbour, BuildingAnimationType::Constructing}, { 42, 43 } },
        { {BuildingType::Harbour, BuildingAnimationType::Built}, { 44, 44 } },
        { {BuildingType::WarningTower, BuildingAnimationType::Constructing}, { 45, 46 } },
        { {BuildingType::WarningTower, BuildingAnimationType::Built}, { 47, 47 } },
        { {BuildingType::TreeWall, BuildingAnimationType::Constructing}, { 48, 48 } },
        { {BuildingType::TreeWall, BuildingAnimationType::Built}, { 48, 48 } },

        // Mappings for Greenskin buildings
        { {BuildingType::Fortress, BuildingAnimationType::Constructing}, { 0, 1 } },
        { {BuildingType::Fortress, BuildingAnimationType::Built}, { 2, 2 } },
        { {BuildingType::HoardKeep, BuildingAnimationType::Constructing}, { 3, 4 } },
        { {BuildingType::HoardKeep, BuildingAnimationType::Built}, { 5, 5 } },
        { {BuildingType::Fort, BuildingAnimationType::Constructing}, { 6, 7 } },
        { {BuildingType::Fort, BuildingAnimationType::Built}, { 8, 8 } },
        { {BuildingType::Blacksmith, BuildingAnimationType::Constructing}, { 9, 10 } },
        { {BuildingType::Blacksmith, BuildingAnimationType::Built}, { 11, 11 } },
        { {BuildingType::BattleQuarters, BuildingAnimationType::Constructing}, { 12, 13 } },
        { {BuildingType::BattleQuarters, BuildingAnimationType::Built}, { 14, 14 } },
        { {BuildingType::BlackNest, BuildingAnimationType::Constructing}, { 15, 16 } },
        { {BuildingType::BlackNest, BuildingAnimationType::Built}, { 17, 17 } },
        { {BuildingType::WeirdWorkshop, BuildingAnimationType::Constructing}, { 18, 19 } },
        { {BuildingType::WeirdWorkshop, BuildingAnimationType::Built}, { 20, 20 } },
        { {BuildingType::UnholyChapel, BuildingAnimationType::Constructing}, { 21, 22 } },
        { {BuildingType::UnholyChapel, BuildingAnimationType::Built}, { 23, 23 } },
        { {BuildingType::AltarOfDoom, BuildingAnimationType::Constructing}, { 24, 25 } },
        { {BuildingType::AltarOfDoom, BuildingAnimationType::Built}, { 26, 26 } },
        { {BuildingType::Docks, BuildingAnimationType::Constructing}, { 27, 28 } },
        { {BuildingType::Docks, BuildingAnimationType::Built}, { 29, 29 } },
        { {BuildingType::GuardTower, BuildingAnimationType::Constructing}, { 30, 31 } },
        { {BuildingType::GuardTower, BuildingAnimationType::Built}, { 32, 32 } },
        { {BuildingType::GreenskinWall, BuildingAnimationType::Constructing}, { 33, 33 } },
        { {BuildingType::GreenskinWall, BuildingAnimationType::Built}, { 33, 33 } },

        // Mappings for Human buildings
        { {BuildingType::Castle, BuildingAnimationType::Constructing}, { 0, 1 } },
        { {BuildingType::Castle, BuildingAnimationType::Built}, { 2, 2 } },
        { {BuildingType::GoldMill, BuildingAnimationType::Constructing}, { 3, 4 } },
        { {BuildingType::GoldMill, BuildingAnimationType::Built}, { 5, 5 } },
        { {BuildingType::ArcheryRange, BuildingAnimationType::Constructing}, { 6, 7 } },
        { {BuildingType::ArcheryRange, BuildingAnimationType::Built}, { 8, 8 } },
        { {BuildingType::Armoury, BuildingAnimationType::Constructing}, { 9, 10 } },
        { {BuildingType::Armoury, BuildingAnimationType::Built}, { 11, 11 } },
        { {BuildingType::Barracks, BuildingAnimationType::Constructing}, { 12, 13 } },
        { {BuildingType::Barracks, BuildingAnimationType::Built}, { 14, 14 } },
        { {BuildingType::HolyStables, BuildingAnimationType::Constructing}, { 15, 16 } },
        { {BuildingType::HolyStables, BuildingAnimationType::Built}, { 17, 17 } },
        { {BuildingType::FireGuild, BuildingAnimationType::Constructing}, { 18, 19 } },
        { {BuildingType::FireGuild, BuildingAnimationType::Built}, { 20, 20 } },
        { {BuildingType::Temple, BuildingAnimationType::Constructing}, { 21, 22 } },
        { {BuildingType::Temple, BuildingAnimationType::Built}, { 23, 32 } },
        { {BuildingType::MageTower, BuildingAnimationType::Constructing}, { 33, 34 } },
        { {BuildingType::MageTower, BuildingAnimationType::Built}, { 35, 35 } },
        { {BuildingType::Shipyard, BuildingAnimationType::Constructing}, { 36, 37 } },
        { {BuildingType::Shipyard, BuildingAnimationType::Built}, { 38, 38 } },
        { {BuildingType::WatchTower, BuildingAnimationType::Constructing}, { 39, 40 } },
        { {BuildingType::WatchTower, BuildingAnimationType::Built}, { 41, 41 } },
        { {BuildingType::Wall, BuildingAnimationType::Constructing}, { 42, 42 } },
        { {BuildingType::Wall, BuildingAnimationType::Built}, { 42, 42 } }
    };

}

#endif // UNIT_ANIMATION_LOOKUP_H
