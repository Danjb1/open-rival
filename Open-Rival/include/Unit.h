#ifndef UNIT_H
#define UNIT_H

#include "Renderable.h"
#include "Sprite.h"

namespace Rival {

    class Unit {

    public:

        const enum Type {

            // Human
            Peasant,
            Bowman,
            LightCavalry,
            Knight,
            FireMaster,
            Thief,
            Ballista,
            ChariotofWar,
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
            PriestofDoom,
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
            MagicChopper
        };

        const enum Facing {
            South,
            SouthWest,
            West,
            NorthWest,
            North,
            NorthEast,
            East,
            SouthEast
        };

        Unit(const Type type);

        void addedToScene(const int newId);

        const Type getType() const;

        const Facing getFacing() const;

        void setFacing(Facing newFacing);

        void rotateLeft();

        void rotateRight();

        const bool isDeleted();

        void markForDeletion();

        const int getId();

    private:

        int id = -1;

        Type type;

        bool deleted = false;

        Facing facing = Facing::South;

    };

}

#endif // UNIT_H
