#include "pch.h"
#include "Scenario.h"

namespace Rival {

    Scenario::Scenario(const int width, const int height, bool wilderness) :
            width(width),
            height(height),
            wilderness(wilderness),
            tiles(std::vector<int>(width * height, 0)) {}

    int Scenario::getTile(const int x, const int y) const {
        return tiles[y * width + x];
    }

}
