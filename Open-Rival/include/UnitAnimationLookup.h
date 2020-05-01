#ifndef UNIT_ANIMATION_LOOKUP_H
#define UNIT_ANIMATION_LOOKUP_H

#include <map>

namespace Rival {

    enum class UnitType : int {

        // Human
        Peasant,
        Bowman,
        LightCavalry,
        Knight,
        FireMaster,
        Thief,
        Ballista,
        ChariotOfWar,
        Wizard,
        Priest,
        SeaBarge,
        Battleship,
        PegasRider,
        Zeppelin,

        // Greenskin
        Serf,
        RockThrower,
        HordeRider,
        Warlord,
        GnomeBoomer,
        Rogue,
        Catapult,
        StormTrooper,
        PriestOfDoom,
        Necromancer,
        LandingCraft,
        TrollGalley,
        Warbat,
        Balloon,

        // Elf
        Yeoman,
        Archer,
        Druid,
        Centaur,
        DwarfMiner,
        Scout,
        Bombard,
        Arquebusier,
        Mage,
        Enchanter,
        Bark,
        Warship,
        SkyRider,
        MagicChopper,

        // Monsters
        Devil,
        Dragon,
        Golem,
        Gryphon,
        Hydra,
        SeaMonster,
        Skeleton,
        Snake
    };

    enum class Facing : int {
        South,
        SouthWest,
        West,
        NorthWest,
        North,
        NorthEast,
        East,
        SouthEast
    };

    enum class UnitAnimationType : int {
        Standing,
        HoldingBag,
        Moving, // eg. walking empty handed for human peasant
        Moving2, // eg. walking with bag for human peasant
        Attacking, // Same as mining for human peasant
        Dying
    };

    // Spritesheet entry start index and end index for
    //   each pair of UnitAnimationType and Facing
    // Providing just for Facing::South case is enough
    using UnitAnimationPair = std::pair<UnitType, UnitAnimationType>;
    using SpritesheetEntry = std::pair<int, int>;
    using UnitAnimationLookup = std::map<UnitAnimationPair, SpritesheetEntry>;
    static const UnitAnimationLookup unitAnimationLookup = {
        // Just human peasant is complete, for proof of concept. A recheck maybe.
        { {UnitType::Peasant, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Peasant, UnitAnimationType::HoldingBag}, { 8, 8 } },
        { {UnitType::Peasant, UnitAnimationType::Moving}, { 16, 21 } },
        { {UnitType::Peasant, UnitAnimationType::Moving2}, { 64, 69 } },
        { {UnitType::Peasant, UnitAnimationType::Attacking}, { 112, 118 } },
        { {UnitType::Peasant, UnitAnimationType::Dying}, { 16, 21 } },

        // This definitely needs a recheck
        { {UnitType::Bowman, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Bowman, UnitAnimationType::Moving}, { 8, 15 } },
        { {UnitType::Bowman, UnitAnimationType::Attacking}, { 72, 80 } },

        { {UnitType::LightCavalry, UnitAnimationType::Standing}, { 0, 0 } },

        { {UnitType::Knight, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Knight, UnitAnimationType::Moving}, { 8, 15 } },
        { {UnitType::Knight, UnitAnimationType::Attacking}, { 72, 79 } },

        { {UnitType::FireMaster, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::FireMaster, UnitAnimationType::Moving}, { 8, 13 } },
        { {UnitType::FireMaster, UnitAnimationType::Attacking}, { 55, 58 } },

        { {UnitType::Thief, UnitAnimationType::Standing}, { 0, 0 } },

        { {UnitType::Ballista, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Ballista, UnitAnimationType::Moving}, { 0, 2 } },
        { {UnitType::Ballista, UnitAnimationType::Attacking}, { 24, 25 } },

        { {UnitType::ChariotOfWar, UnitAnimationType::Standing}, { 0, 0 } },

        { {UnitType::Wizard, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Wizard, UnitAnimationType::Moving}, { 8, 13 } },
        { {UnitType::Wizard, UnitAnimationType::Attacking}, { 56, 58 } },

        { {UnitType::Priest, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::SeaBarge, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Battleship, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::PegasRider, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Zeppelin, UnitAnimationType::Standing}, { 0, 0 } },

        { {UnitType::Serf, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::RockThrower, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::HordeRider, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Warlord, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::GnomeBoomer, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Rogue, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Catapult, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::StormTrooper, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::PriestOfDoom, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Necromancer, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::LandingCraft, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::TrollGalley, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Warbat, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Balloon, UnitAnimationType::Standing}, { 0, 0 } },

        { {UnitType::Yeoman, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Archer, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Druid, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Centaur, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::DwarfMiner, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Scout, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Bombard, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Arquebusier, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Mage, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Enchanter, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Bark, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Warship, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::SkyRider, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::MagicChopper, UnitAnimationType::Standing}, { 0, 0 } },

        { {UnitType::Devil, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Dragon, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Golem, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Gryphon, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Hydra, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::SeaMonster, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Skeleton, UnitAnimationType::Standing}, { 0, 0 } },
        { {UnitType::Snake, UnitAnimationType::Standing}, { 0, 0 } },
    };

}

#endif // UNIT_ANIMATION_LOOKUP_H
