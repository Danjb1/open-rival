#ifndef UNIT_H
#define UNIT_H

namespace Rival {
namespace Unit {

    enum class Type {

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

    static const int firstUnitType = static_cast<int>(Type::Ballista);
    static const int lastUnitType = static_cast<int>(Type::Snake);

    // Size, in tiles
    static const int width = 1;
    static const int height = 1;
}
}  // namespace Rival::Unit

#endif  // UNIT_H
