#ifndef SCENARIO_H
#define SCENARIO_H

#include <map>
#include <vector>

#include "Unit.h"

namespace Rival {

    class Scenario {

    public:

        Scenario(const int width, const int height, bool wilderness);

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
