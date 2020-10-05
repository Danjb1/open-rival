#ifndef UNIT_TYPES_H
#define UNIT_TYPES_H

namespace Rival {

    enum class UnitType {

        // Human
        Ballista,
        Battleship,
        Bowman,
        ChariotOfWar,
        FireMaster,
        Knight,
        LightCavalry,
        Peasant,
        PegasRider,
        Priest,
        SeaBarge,
        Thief,
        Wizard,
        Zeppelin,

        // Greenskin
        Balloon,
        Catapult,
        GnomeBoomer,
        HordeRider,
        LandingCraft,
        Necromancer,
        PriestOfDoom,
        RockThrower,
        Rogue,
        Serf,
        StormTrooper,
        TrollGalley,
        Warbat,
        Warlord,

        // Elf
        Archer,
        Arquebusier,
        Bark,
        Bombard,
        Centaur,
        Druid,
        DwarfMiner,
        Enchanter,
        Mage,
        MagicChopper,
        Scout,
        SkyRider,
        Warship,
        Yeoman,

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

    const auto firstUnitType { static_cast<int>(UnitType::Ballista) };
    const auto lastUnitType { static_cast<int>(UnitType::Snake) };

}  // namespace Rival

#endif  // UNIT_TYPES_H
