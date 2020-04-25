#ifndef SCENARIO_H
#define SCENARIO_H

#include <map>
#include <vector>

#include "Tile.h"
#include "Unit.h"

namespace Rival {

    class Scenario {

    public:

        // Creates a blank Scenario
        Scenario(int width, int height, bool wilderness);

        int getWidth() const;

        int getHeight() const;

        const std::vector<Tile>& getTiles();

        // Sets all tiles at once
        void tilesLoaded(std::vector<Tile> tiles);

        Tile getTile(int x, int y) const;

        bool isWilderness() const;

        void addUnit(
                std::unique_ptr<Unit> unit,
                int player,
                int x,
                int y,
                Facing facing);

        std::map<int, std::unique_ptr<Unit>>& getUnits();

    private:

        const int width;
        const int height;
        bool wilderness;
        std::vector<Tile> tiles;
        std::vector<TilePassability> tilePassability;

        int nextId;
        std::map<int, std::unique_ptr<Unit>> units;

        void setPassability(int x, int y, TilePassability passability);

    };

}

#endif // SCENARIO_H
