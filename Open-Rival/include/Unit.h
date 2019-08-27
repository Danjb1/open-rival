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

        Unit(const Type type, int x, int y);

        void addedToScene(const int newId);

        const Type getType() const;

        const Facing getFacing() const;

        void setFacing(Facing newFacing);

        void rotateLeft();

        void rotateRight();

        const bool isDeleted();

        void markForDeletion();

        const int getId();

        int getX() const;

        int getY() const;

    private:

        int id = -1;

        Type type;

        bool deleted = false;

        int x;

        int y;

        Facing facing = Facing::South;

    };

}

#endif // UNIT_H
