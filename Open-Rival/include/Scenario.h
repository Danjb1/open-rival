#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>

namespace Rival {

    class Scenario {

    public:

        Scenario(const int width, const int height, bool wilderness);

        int getTile(const int x, const int y) const;

    private:

        const int width;
        const int height;
        bool wilderness;

        std::vector<int> tiles;

    };

}

#endif // SCENARIO_H
