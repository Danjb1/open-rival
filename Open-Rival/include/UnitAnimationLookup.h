#ifndef UNIT_ANIMATION_LOOKUP_H
#define UNIT_ANIMATION_LOOKUP_H

#include <map>

#include "TimerUtils.h"
#include "UnitTypes.h"

namespace Rival {

    enum class Facing {
        South,
        SouthWest,
        West,
        NorthWest,
        North,
        NorthEast,
        East,
        SouthEast
    };

    enum class UnitAnimationType {
        Standing,
        HoldingBag,
        Moving,
        MovingWithBag,
        Attacking,
        Dying
    };

    using UnitAnimationPair = std::pair<UnitType, UnitAnimationType>;
    // UnitSpriteSheetEntry holds start index and end index for
    //   each pair of UnitAnimationType and Facing, together with
    //   time needed between animation steps.
    // Providing this data just for Facing::South case is enough
    using UnitSpritesheetEntry = std::tuple<int, int, TimerUtils::MillisecondsRep>;
    using UnitAnimationLookup = std::map<UnitAnimationPair, UnitSpritesheetEntry>;
    using TimerUtils::toMS;
    static const UnitAnimationLookup unitAnimationLookup = {
        // Just human peasant is complete, for proof of concept. A recheck maybe.
        { {UnitType::Peasant, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Peasant, UnitAnimationType::HoldingBag}, { 8, 8, toMS(0) } },
        { {UnitType::Peasant, UnitAnimationType::Moving}, { 16, 21, toMS(250) } },
        { {UnitType::Peasant, UnitAnimationType::MovingWithBag}, { 64, 69, toMS(250) } },
        { {UnitType::Peasant, UnitAnimationType::Attacking}, { 112, 118, toMS(250) } },
        { {UnitType::Peasant, UnitAnimationType::Dying}, { 16, 21, toMS(500) } },

        // This definitely needs a recheck
        { {UnitType::Bowman, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Bowman, UnitAnimationType::Moving}, { 8, 15, toMS(250) } },
        { {UnitType::Bowman, UnitAnimationType::Attacking}, { 72, 80, toMS(250) } },

        { {UnitType::LightCavalry, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },

        { {UnitType::Knight, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Knight, UnitAnimationType::Moving}, { 8, 15, toMS(250) } },
        { {UnitType::Knight, UnitAnimationType::Attacking}, { 72, 79, toMS(250) } },

        { {UnitType::FireMaster, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::FireMaster, UnitAnimationType::Moving}, { 8, 13, toMS(250) } },
        { {UnitType::FireMaster, UnitAnimationType::Attacking}, { 55, 58, toMS(250) } },

        { {UnitType::Thief, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },

        { {UnitType::Ballista, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Ballista, UnitAnimationType::Moving}, { 0, 2, toMS(250) } },
        { {UnitType::Ballista, UnitAnimationType::Attacking}, { 24, 25, toMS(250) } },

        { {UnitType::ChariotOfWar, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },

        { {UnitType::Wizard, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Wizard, UnitAnimationType::Moving}, { 8, 13, toMS(250) } },
        { {UnitType::Wizard, UnitAnimationType::Attacking}, { 56, 58, toMS(250) } },

        { {UnitType::Priest, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::SeaBarge, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Battleship, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::PegasRider, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Zeppelin, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },

        { {UnitType::Serf, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::RockThrower, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::HordeRider, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Warlord, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::GnomeBoomer, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Rogue, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Catapult, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::StormTrooper, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::PriestOfDoom, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Necromancer, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::LandingCraft, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::TrollGalley, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Warbat, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Balloon, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },

        { {UnitType::Yeoman, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Archer, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Druid, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Centaur, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::DwarfMiner, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Scout, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Bombard, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Arquebusier, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Mage, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Enchanter, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Bark, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Warship, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::SkyRider, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::MagicChopper, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },

        { {UnitType::Devil, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Dragon, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Golem, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Gryphon, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Hydra, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::SeaMonster, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Skeleton, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
        { {UnitType::Snake, UnitAnimationType::Standing}, { 0, 0, toMS(0) } },
    };

}

#endif // UNIT_ANIMATION_LOOKUP_H
