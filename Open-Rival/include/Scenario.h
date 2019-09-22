#ifndef SCENARIO_H
#define SCENARIO_H

#include <map>
#include <vector>

#include "Unit.h"

namespace Rival {

    enum TileType {
        Grass,
        Coastline,
        Water,
        Mud,
        Dirt,
        Dungeon,
        Gold,
        Cropland
    };

    class Scenario {

    public:

        Scenario(const int width, const int height, bool wilderness);

        int getWidth() const;

        int getHeight() const;

        std::vector<int>& getTiles();

        int getTile(const int x, const int y) const;

        bool isWilderness() const;

        void addUnit(std::unique_ptr<Unit> unit);

        std::map<int, std::unique_ptr<Unit>>& getUnits();

    private:

        const int width;
        const int height;
        bool wilderness;
        std::vector<int> tiles;

        int nextId;
        std::map<int, std::unique_ptr<Unit>> units;

    };

}

#endif // SCENARIO_H
