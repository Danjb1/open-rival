#ifndef UNIT_ANIMATION_LOOKUP_H
#define UNIT_ANIMATION_LOOKUP_H

#include <map>

#include "TimerUtils.h"
#include "UnitTypes.h"

namespace Rival {

    enum class UnitAnimationType {
        Standing,
        HoldingBag,
        Moving,
        MovingWithBag,
        Attacking,
        Dying
    };

    using UnitAnimationPair = std::pair<UnitType, UnitAnimationType>;

    // { start index, end index, msPerFrame }
    // Providing this data just for Facing::South case is enough
    using UnitSpritesheetEntry = std::tuple<int, int, int>;

    using UnitAnimationLookup = std::map<UnitAnimationPair, UnitSpritesheetEntry>;

    static const UnitAnimationLookup unitAnimationLookup = {
        // Just human peasant is complete, for proof of concept. A recheck maybe.
        { { UnitType::Peasant, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Peasant, UnitAnimationType::HoldingBag }, { 8, 8, 1 } },
        { { UnitType::Peasant, UnitAnimationType::Moving }, { 16, 21, 8 } },
        { { UnitType::Peasant, UnitAnimationType::MovingWithBag }, { 64, 69, 8 } },
        { { UnitType::Peasant, UnitAnimationType::Attacking }, { 112, 118, 8 } },
        { { UnitType::Peasant, UnitAnimationType::Dying }, { 16, 21, 15 } },

        // This definitely needs a recheck
        { { UnitType::Bowman, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Bowman, UnitAnimationType::Moving }, { 8, 15, 8 } },
        { { UnitType::Bowman, UnitAnimationType::Attacking }, { 72, 80, 8 } },

        { { UnitType::LightCavalry, UnitAnimationType::Standing }, { 0, 0, 1 } },

        { { UnitType::Knight, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Knight, UnitAnimationType::Moving }, { 8, 15, 4 } },
        { { UnitType::Knight, UnitAnimationType::Attacking }, { 72, 79, 4 } },

        { { UnitType::FireMaster, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::FireMaster, UnitAnimationType::Moving }, { 8, 13, 8 } },
        { { UnitType::FireMaster, UnitAnimationType::Attacking }, { 55, 58, 8 } },

        { { UnitType::Thief, UnitAnimationType::Standing }, { 0, 0, 1 } },

        { { UnitType::Ballista, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Ballista, UnitAnimationType::Moving }, { 0, 2, 8 } },
        { { UnitType::Ballista, UnitAnimationType::Attacking }, { 24, 25, 30 } },

        { { UnitType::ChariotOfWar, UnitAnimationType::Standing }, { 0, 0, 1 } },

        { { UnitType::Wizard, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Wizard, UnitAnimationType::Moving }, { 8, 13, 8 } },
        { { UnitType::Wizard, UnitAnimationType::Attacking }, { 56, 58, 8 } },

        { { UnitType::Priest, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::SeaBarge, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Battleship, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::PegasRider, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Zeppelin, UnitAnimationType::Standing }, { 0, 0, 1 } },

        { { UnitType::Serf, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::RockThrower, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::HordeRider, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Warlord, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::GnomeBoomer, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Rogue, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Catapult, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::StormTrooper, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::PriestOfDoom, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Necromancer, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::LandingCraft, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::TrollGalley, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Warbat, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Balloon, UnitAnimationType::Standing }, { 0, 0, 1 } },

        { { UnitType::Yeoman, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Archer, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Druid, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Centaur, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::DwarfMiner, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Scout, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Bombard, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Arquebusier, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Mage, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Enchanter, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Bark, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Warship, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::SkyRider, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::MagicChopper, UnitAnimationType::Standing }, { 0, 0, 1 } },

        { { UnitType::Devil, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Dragon, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Golem, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Gryphon, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Hydra, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::SeaMonster, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Skeleton, UnitAnimationType::Standing }, { 0, 0, 1 } },
        { { UnitType::Snake, UnitAnimationType::Standing }, { 0, 0, 1 } },
    };

}  // namespace Rival

#endif  // UNIT_ANIMATION_LOOKUP_H
