#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <map>

#include "Building.h"
#include "Unit.h"

namespace Rival {

    enum class UnitAnimationType {
        Standing,
        HoldingBag,
        Moving,
        MovingWithBag,
        Attacking,
        Dying
    };

    // We have 16 wall variants.
    // The underlying int value corresponds to the wall's texture index.
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

    struct Animation {
        int startIndex;
        int endIndex;
        int msPerFrame;
    };

    using UnitAnimationPair = std::pair<Unit::Type, UnitAnimationType>;
    using BuildingAnimationPair = std::pair<Building::Type, BuildingAnimationType>;

    /**
     * Unit animation definitions.
     *
     * Providing this data just for Facing::South is sufficient
     */
    static const std::map<UnitAnimationPair, Animation> unitAnimationLookup = {
        { { Unit::Type::Peasant, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Peasant, UnitAnimationType::HoldingBag }, { 8, 8, 1 } },
        { { Unit::Type::Peasant, UnitAnimationType::Moving }, { 16, 21, 8 } },
        { { Unit::Type::Peasant, UnitAnimationType::MovingWithBag }, { 64, 69, 8 } },
        { { Unit::Type::Peasant, UnitAnimationType::Attacking }, { 112, 118, 8 } },
        { { Unit::Type::Peasant, UnitAnimationType::Dying }, { 16, 21, 15 } },

        { { Unit::Type::Bowman, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Bowman, UnitAnimationType::Moving }, { 8, 15, 8 } },
        { { Unit::Type::Bowman, UnitAnimationType::Attacking }, { 72, 80, 8 } },

        { { Unit::Type::LightCavalry, UnitAnimationType::Standing }, { 0, 0, 1 } },

        { { Unit::Type::Knight, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Knight, UnitAnimationType::Moving }, { 8, 15, 4 } },
        { { Unit::Type::Knight, UnitAnimationType::Attacking }, { 72, 79, 4 } },

        { { Unit::Type::FireMaster, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::FireMaster, UnitAnimationType::Moving }, { 8, 13, 8 } },
        { { Unit::Type::FireMaster, UnitAnimationType::Attacking }, { 55, 58, 8 } },

        { { Unit::Type::Thief, UnitAnimationType::Standing }, { 0, 0, 1 } },

        { { Unit::Type::Ballista, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Ballista, UnitAnimationType::Moving }, { 0, 2, 8 } },
        { { Unit::Type::Ballista, UnitAnimationType::Attacking }, { 24, 25, 30 } },

        { { Unit::Type::ChariotOfWar, UnitAnimationType::Standing }, { 0, 0, 1 } },

        { { Unit::Type::Wizard, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Wizard, UnitAnimationType::Moving }, { 8, 13, 8 } },
        { { Unit::Type::Wizard, UnitAnimationType::Attacking }, { 56, 58, 8 } },

        { { Unit::Type::Priest, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::SeaBarge, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Battleship, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::PegasRider, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Zeppelin, UnitAnimationType::Standing }, { 0, 0, 1 } },

        { { Unit::Type::Serf, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::RockThrower, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::HordeRider, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Warlord, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::GnomeBoomer, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Rogue, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Catapult, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::StormTrooper, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::PriestOfDoom, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Necromancer, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::LandingCraft, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::TrollGalley, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Warbat, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Balloon, UnitAnimationType::Standing }, { 0, 0, 1 } },

        { { Unit::Type::Yeoman, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Archer, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Druid, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Centaur, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::DwarfMiner, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Scout, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Bombard, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Arquebusier, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Mage, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Enchanter, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Bark, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Warship, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::SkyRider, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::MagicChopper, UnitAnimationType::Standing }, { 0, 0, 1 } },

        { { Unit::Type::Devil, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Dragon, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Golem, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Gryphon, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Hydra, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::SeaMonster, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Skeleton, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { Unit::Type::Snake, UnitAnimationType::Standing }, { 0, 0, 1 } },
    };

    /**
     * Building animation definitions.
     */
    static const std::map<BuildingAnimationPair, Animation> buildingAnimationLookup = {

        // Elf
        { { Building::Type::ElvenKeep, BuildingAnimationType::Constructing }, { 0, 1, 1 } },
        { { Building::Type::ElvenKeep, BuildingAnimationType::Built }, { 2, 2, 1 } },
        { { Building::Type::Treasury, BuildingAnimationType::Constructing }, { 3, 4, 1 } },
        { { Building::Type::Treasury, BuildingAnimationType::Built }, { 5, 11, 1 } },
        { { Building::Type::CombatCamp, BuildingAnimationType::Constructing }, { 12, 13, 1 } },
        { { Building::Type::CombatCamp, BuildingAnimationType::Built }, { 14, 14, 1 } },
        { { Building::Type::Arsenal, BuildingAnimationType::Constructing }, { 15, 16, 1 } },
        { { Building::Type::Arsenal, BuildingAnimationType::Built }, { 17, 17, 1 } },
        { { Building::Type::DuelRange, BuildingAnimationType::Constructing }, { 18, 19, 1 } },
        { { Building::Type::DuelRange, BuildingAnimationType::Built }, { 20, 20, 1 } },
        { { Building::Type::HolyNest, BuildingAnimationType::Constructing }, { 21, 22, 1 } },
        { { Building::Type::HolyNest, BuildingAnimationType::Built }, { 23, 23, 1 } },
        { { Building::Type::MinerGuildhall, BuildingAnimationType::Constructing }, { 24, 25, 1 } },
        { { Building::Type::MinerGuildhall, BuildingAnimationType::Built }, { 26, 26, 1 } },
        { { Building::Type::AbbeyTower, BuildingAnimationType::Constructing }, { 27, 28, 1 } },
        { { Building::Type::AbbeyTower, BuildingAnimationType::Built }, { 29, 29, 1 } },
        { { Building::Type::CouncilOfRunes, BuildingAnimationType::Constructing }, { 30, 31, 1 } },
        { { Building::Type::CouncilOfRunes, BuildingAnimationType::Built }, { 32, 41, 1 } },
        { { Building::Type::Harbour, BuildingAnimationType::Constructing }, { 42, 43, 1 } },
        { { Building::Type::Harbour, BuildingAnimationType::Built }, { 44, 44, 1 } },
        { { Building::Type::WarningTower, BuildingAnimationType::Constructing }, { 45, 46, 1 } },
        { { Building::Type::WarningTower, BuildingAnimationType::Built }, { 47, 47, 1 } },
        { { Building::Type::TreeWall, BuildingAnimationType::Constructing }, { 48, 48, 1 } },
        { { Building::Type::TreeWall, BuildingAnimationType::Built }, { 48, 48, 1 } },

        // Greenskin
        { { Building::Type::Fortress, BuildingAnimationType::Constructing }, { 0, 1, 1 } },
        { { Building::Type::Fortress, BuildingAnimationType::Built }, { 2, 2, 1 } },
        { { Building::Type::HoardKeep, BuildingAnimationType::Constructing }, { 3, 4, 1 } },
        { { Building::Type::HoardKeep, BuildingAnimationType::Built }, { 5, 5, 1 } },
        { { Building::Type::Fort, BuildingAnimationType::Constructing }, { 6, 7, 1 } },
        { { Building::Type::Fort, BuildingAnimationType::Built }, { 8, 8, 1 } },
        { { Building::Type::Blacksmith, BuildingAnimationType::Constructing }, { 9, 10, 1 } },
        { { Building::Type::Blacksmith, BuildingAnimationType::Built }, { 11, 11, 1 } },
        { { Building::Type::BattleQuarters, BuildingAnimationType::Constructing }, { 12, 13, 1 } },
        { { Building::Type::BattleQuarters, BuildingAnimationType::Built }, { 14, 14, 1 } },
        { { Building::Type::BlackNest, BuildingAnimationType::Constructing }, { 15, 16, 1 } },
        { { Building::Type::BlackNest, BuildingAnimationType::Built }, { 17, 17, 1 } },
        { { Building::Type::WeirdWorkshop, BuildingAnimationType::Constructing }, { 18, 19, 1 } },
        { { Building::Type::WeirdWorkshop, BuildingAnimationType::Built }, { 20, 20, 1 } },
        { { Building::Type::UnholyChapel, BuildingAnimationType::Constructing }, { 21, 22, 1 } },
        { { Building::Type::UnholyChapel, BuildingAnimationType::Built }, { 23, 23, 1 } },
        { { Building::Type::AltarOfDoom, BuildingAnimationType::Constructing }, { 24, 25, 1 } },
        { { Building::Type::AltarOfDoom, BuildingAnimationType::Built }, { 26, 26, 1 } },
        { { Building::Type::Docks, BuildingAnimationType::Constructing }, { 27, 28, 1 } },
        { { Building::Type::Docks, BuildingAnimationType::Built }, { 29, 29, 1 } },
        { { Building::Type::GuardTower, BuildingAnimationType::Constructing }, { 30, 31, 1 } },
        { { Building::Type::GuardTower, BuildingAnimationType::Built }, { 32, 32, 1 } },
        { { Building::Type::GreenskinWall, BuildingAnimationType::Constructing }, { 33, 33, 1 } },
        { { Building::Type::GreenskinWall, BuildingAnimationType::Built }, { 33, 33, 1 } },

        // Human
        { { Building::Type::Castle, BuildingAnimationType::Constructing }, { 0, 1, 1 } },
        { { Building::Type::Castle, BuildingAnimationType::Built }, { 2, 2, 1 } },
        { { Building::Type::GoldMill, BuildingAnimationType::Constructing }, { 3, 4, 1 } },
        { { Building::Type::GoldMill, BuildingAnimationType::Built }, { 5, 5, 1 } },
        { { Building::Type::ArcheryRange, BuildingAnimationType::Constructing }, { 6, 7, 1 } },
        { { Building::Type::ArcheryRange, BuildingAnimationType::Built }, { 8, 8, 1 } },
        { { Building::Type::Armoury, BuildingAnimationType::Constructing }, { 9, 10, 1 } },
        { { Building::Type::Armoury, BuildingAnimationType::Built }, { 11, 11, 1 } },
        { { Building::Type::Barracks, BuildingAnimationType::Constructing }, { 12, 13, 1 } },
        { { Building::Type::Barracks, BuildingAnimationType::Built }, { 14, 14, 1 } },
        { { Building::Type::HolyStables, BuildingAnimationType::Constructing }, { 15, 16, 1 } },
        { { Building::Type::HolyStables, BuildingAnimationType::Built }, { 17, 17, 1 } },
        { { Building::Type::FireGuild, BuildingAnimationType::Constructing }, { 18, 19, 1 } },
        { { Building::Type::FireGuild, BuildingAnimationType::Built }, { 20, 20, 1 } },
        { { Building::Type::Temple, BuildingAnimationType::Constructing }, { 21, 22, 1 } },
        { { Building::Type::Temple, BuildingAnimationType::Built }, { 23, 32, 1 } },
        { { Building::Type::MageTower, BuildingAnimationType::Constructing }, { 33, 34, 1 } },
        { { Building::Type::MageTower, BuildingAnimationType::Built }, { 35, 35, 1 } },
        { { Building::Type::Shipyard, BuildingAnimationType::Constructing }, { 36, 37, 1 } },
        { { Building::Type::Shipyard, BuildingAnimationType::Built }, { 38, 38, 1 } },
        { { Building::Type::WatchTower, BuildingAnimationType::Constructing }, { 39, 40, 1 } },
        { { Building::Type::WatchTower, BuildingAnimationType::Built }, { 41, 41, 1 } },
        { { Building::Type::Wall, BuildingAnimationType::Constructing }, { 42, 42, 1 } },
        { { Building::Type::Wall, BuildingAnimationType::Built }, { 42, 42, 1 } }
    };

}  // namespace Rival

#endif  // ANIMATIONS_H
