#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <map>

#include "Building.h"
#include "Unit.h"

namespace Rival {
namespace Animations {

    enum class UnitAnimationType {
        Standing,
        HoldingBag,
        Moving,
        MovingWithBag,
        Attacking,
        Dying
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
     * Providing this data just for Facing::South is sufficient.
     */
    static const std::map<UnitAnimationPair, const Animation> unitAnimationLookup = {
        { { Unit::Type::Peasant, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Peasant, UnitAnimationType::HoldingBag }, { 8, 8, 0 } },
        { { Unit::Type::Peasant, UnitAnimationType::Moving }, { 16, 21, 60 } },
        { { Unit::Type::Peasant, UnitAnimationType::MovingWithBag }, { 64, 69, 60 } },
        { { Unit::Type::Peasant, UnitAnimationType::Attacking }, { 112, 118, 60 } },
        { { Unit::Type::Peasant, UnitAnimationType::Dying }, { 16, 21, 60 } },

        { { Unit::Type::Bowman, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Bowman, UnitAnimationType::Moving }, { 8, 15, 60 } },
        { { Unit::Type::Bowman, UnitAnimationType::Attacking }, { 72, 80, 60 } },

        { { Unit::Type::LightCavalry, UnitAnimationType::Standing }, { 0, 0, 0 } },

        { { Unit::Type::Knight, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Knight, UnitAnimationType::Moving }, { 8, 15, 0 } },
        { { Unit::Type::Knight, UnitAnimationType::Attacking }, { 72, 79, 0 } },

        { { Unit::Type::FireMaster, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::FireMaster, UnitAnimationType::Moving }, { 8, 13, 0 } },
        { { Unit::Type::FireMaster, UnitAnimationType::Attacking }, { 55, 58, 0 } },

        { { Unit::Type::Thief, UnitAnimationType::Standing }, { 0, 0, 0 } },

        { { Unit::Type::Ballista, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Ballista, UnitAnimationType::Moving }, { 0, 2, 0 } },
        { { Unit::Type::Ballista, UnitAnimationType::Attacking }, { 24, 25, 0 } },

        { { Unit::Type::ChariotOfWar, UnitAnimationType::Standing }, { 0, 0, 0 } },

        { { Unit::Type::Wizard, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Wizard, UnitAnimationType::Moving }, { 8, 13, 0 } },
        { { Unit::Type::Wizard, UnitAnimationType::Attacking }, { 56, 58, 0 } },

        { { Unit::Type::Priest, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::SeaBarge, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Battleship, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::PegasRider, UnitAnimationType::Standing }, { 0, 0, 60 } },
        { { Unit::Type::Zeppelin, UnitAnimationType::Standing }, { 0, 0, 0 } },

        { { Unit::Type::Serf, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::RockThrower, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::HordeRider, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Warlord, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::GnomeBoomer, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Rogue, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Catapult, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::StormTrooper, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::PriestOfDoom, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Necromancer, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::LandingCraft, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::TrollGalley, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Warbat, UnitAnimationType::Standing }, { 0, 0, 60 } },
        { { Unit::Type::Balloon, UnitAnimationType::Standing }, { 0, 0, 0 } },

        { { Unit::Type::Yeoman, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Archer, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Druid, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Centaur, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::DwarfMiner, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Scout, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Bombard, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Arquebusier, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Mage, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Enchanter, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Bark, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Warship, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::SkyRider, UnitAnimationType::Standing }, { 0, 0, 60 } },
        { { Unit::Type::MagicChopper, UnitAnimationType::Standing }, { 0, 0, 60 } },

        { { Unit::Type::Devil, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Dragon, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Golem, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Gryphon, UnitAnimationType::Standing }, { 0, 0, 60 } },
        { { Unit::Type::Hydra, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::SeaMonster, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Skeleton, UnitAnimationType::Standing }, { 0, 0, 0 } },
        { { Unit::Type::Snake, UnitAnimationType::Standing }, { 0, 0, 60 } },
    };

    /**
     * Building animation definitions.
     */
    static const std::map<BuildingAnimationPair, const Animation> buildingAnimationLookup = {

        // Elf
        { { Building::Type::ElvenKeep, BuildingAnimationType::Constructing }, { 0, 1, 60 } },
        { { Building::Type::ElvenKeep, BuildingAnimationType::Built }, { 2, 2, 0 } },
        { { Building::Type::Treasury, BuildingAnimationType::Constructing }, { 3, 4, 60 } },
        { { Building::Type::Treasury, BuildingAnimationType::Built }, { 5, 11, 0 } },
        { { Building::Type::CombatCamp, BuildingAnimationType::Constructing }, { 12, 13, 60 } },
        { { Building::Type::CombatCamp, BuildingAnimationType::Built }, { 14, 14, 0 } },
        { { Building::Type::Arsenal, BuildingAnimationType::Constructing }, { 15, 16, 60 } },
        { { Building::Type::Arsenal, BuildingAnimationType::Built }, { 17, 17, 0 } },
        { { Building::Type::DuelRange, BuildingAnimationType::Constructing }, { 18, 19, 60 } },
        { { Building::Type::DuelRange, BuildingAnimationType::Built }, { 20, 20, 0 } },
        { { Building::Type::HolyNest, BuildingAnimationType::Constructing }, { 21, 22, 60 } },
        { { Building::Type::HolyNest, BuildingAnimationType::Built }, { 23, 23, 0 } },
        { { Building::Type::MinerGuildhall, BuildingAnimationType::Constructing }, { 24, 25, 60 } },
        { { Building::Type::MinerGuildhall, BuildingAnimationType::Built }, { 26, 26, 0 } },
        { { Building::Type::AbbeyTower, BuildingAnimationType::Constructing }, { 27, 28, 60 } },
        { { Building::Type::AbbeyTower, BuildingAnimationType::Built }, { 29, 29, 0 } },
        { { Building::Type::CouncilOfRunes, BuildingAnimationType::Constructing }, { 30, 31, 60 } },
        { { Building::Type::CouncilOfRunes, BuildingAnimationType::Built }, { 32, 41, 0 } },
        { { Building::Type::Harbour, BuildingAnimationType::Constructing }, { 42, 43, 60 } },
        { { Building::Type::Harbour, BuildingAnimationType::Built }, { 44, 44, 0 } },
        { { Building::Type::WarningTower, BuildingAnimationType::Constructing }, { 45, 46, 60 } },
        { { Building::Type::WarningTower, BuildingAnimationType::Built }, { 47, 47, 0 } },

        // Greenskin
        { { Building::Type::Fortress, BuildingAnimationType::Constructing }, { 0, 1, 60 } },
        { { Building::Type::Fortress, BuildingAnimationType::Built }, { 2, 2, 0 } },
        { { Building::Type::HoardKeep, BuildingAnimationType::Constructing }, { 3, 4, 60 } },
        { { Building::Type::HoardKeep, BuildingAnimationType::Built }, { 5, 5, 0 } },
        { { Building::Type::Fort, BuildingAnimationType::Constructing }, { 6, 7, 60 } },
        { { Building::Type::Fort, BuildingAnimationType::Built }, { 8, 8, 0 } },
        { { Building::Type::Blacksmith, BuildingAnimationType::Constructing }, { 9, 10, 60 } },
        { { Building::Type::Blacksmith, BuildingAnimationType::Built }, { 11, 11, 0 } },
        { { Building::Type::BattleQuarters, BuildingAnimationType::Constructing }, { 12, 13, 60 } },
        { { Building::Type::BattleQuarters, BuildingAnimationType::Built }, { 14, 14, 0 } },
        { { Building::Type::BlackNest, BuildingAnimationType::Constructing }, { 15, 16, 60 } },
        { { Building::Type::BlackNest, BuildingAnimationType::Built }, { 17, 17, 0 } },
        { { Building::Type::WeirdWorkshop, BuildingAnimationType::Constructing }, { 18, 19, 60 } },
        { { Building::Type::WeirdWorkshop, BuildingAnimationType::Built }, { 20, 20, 0 } },
        { { Building::Type::UnholyChapel, BuildingAnimationType::Constructing }, { 21, 22, 60 } },
        { { Building::Type::UnholyChapel, BuildingAnimationType::Built }, { 23, 23, 0 } },
        { { Building::Type::AltarOfDoom, BuildingAnimationType::Constructing }, { 24, 25, 60 } },
        { { Building::Type::AltarOfDoom, BuildingAnimationType::Built }, { 26, 26, 0 } },
        { { Building::Type::Docks, BuildingAnimationType::Constructing }, { 27, 28, 60 } },
        { { Building::Type::Docks, BuildingAnimationType::Built }, { 29, 29, 0 } },
        { { Building::Type::GuardTower, BuildingAnimationType::Constructing }, { 30, 31, 60 } },
        { { Building::Type::GuardTower, BuildingAnimationType::Built }, { 32, 32, 0 } },

        // Human
        { { Building::Type::Castle, BuildingAnimationType::Constructing }, { 0, 1, 60 } },
        { { Building::Type::Castle, BuildingAnimationType::Built }, { 2, 2, 0 } },
        { { Building::Type::GoldMill, BuildingAnimationType::Constructing }, { 3, 4, 60 } },
        { { Building::Type::GoldMill, BuildingAnimationType::Built }, { 5, 5, 0 } },
        { { Building::Type::ArcheryRange, BuildingAnimationType::Constructing }, { 6, 7, 60 } },
        { { Building::Type::ArcheryRange, BuildingAnimationType::Built }, { 8, 8, 0 } },
        { { Building::Type::Armoury, BuildingAnimationType::Constructing }, { 9, 10, 60 } },
        { { Building::Type::Armoury, BuildingAnimationType::Built }, { 11, 11, 0 } },
        { { Building::Type::Barracks, BuildingAnimationType::Constructing }, { 12, 13, 60 } },
        { { Building::Type::Barracks, BuildingAnimationType::Built }, { 14, 14, 0 } },
        { { Building::Type::HolyStables, BuildingAnimationType::Constructing }, { 15, 16, 60 } },
        { { Building::Type::HolyStables, BuildingAnimationType::Built }, { 17, 17, 0 } },
        { { Building::Type::FireGuild, BuildingAnimationType::Constructing }, { 18, 19, 60 } },
        { { Building::Type::FireGuild, BuildingAnimationType::Built }, { 20, 20, 0 } },
        { { Building::Type::Temple, BuildingAnimationType::Constructing }, { 21, 22, 60 } },
        { { Building::Type::Temple, BuildingAnimationType::Built }, { 23, 32, 0 } },
        { { Building::Type::MageTower, BuildingAnimationType::Constructing }, { 33, 34, 60 } },
        { { Building::Type::MageTower, BuildingAnimationType::Built }, { 35, 35, 0 } },
        { { Building::Type::Shipyard, BuildingAnimationType::Constructing }, { 36, 37, 60 } },
        { { Building::Type::Shipyard, BuildingAnimationType::Built }, { 38, 38, 0 } },
        { { Building::Type::WatchTower, BuildingAnimationType::Constructing }, { 39, 40, 60 } },
        { { Building::Type::WatchTower, BuildingAnimationType::Built }, { 41, 41, 0 } },
    };

    Animation getUnitAnimation(
            Unit::Type unitType, UnitAnimationType animType);

    Animation getBuildingAnimation(
            Building::Type buildingType, BuildingAnimationType animType);

}}  // namespace Rival::Animations

#endif  // ANIMATIONS_H
