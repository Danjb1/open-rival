#include "pch.h"
#include "Scenario.h"

namespace Rival {

    Scenario::Scenario(int width, int height, bool wilderness) :
            width(width),
            height(height),
            wilderness(wilderness),
            tiles(std::vector<Tile>(
                    width * height, Tile(TileType::Grass, 0, 0))) {}

    int Scenario::getWidth() const {
        return width;
    }

    int Scenario::getHeight() const {
        return height;
    }

    std::vector<Tile>& Scenario::getTiles() {
        return tiles;
    }

    Tile Scenario::getTile(int x, int y) const {
        return tiles[y * width + x];
    }

    bool Scenario::isWilderness() const {
        return wilderness;
    }

    void Scenario::addUnit(std::unique_ptr<Unit> unit) {
        units[nextId] = std::move(unit);
        units[nextId]->addedToScene(nextId);
        nextId++;
    }

    std::map<int, std::unique_ptr<Unit>>& Scenario::getUnits() {
        return units;
    }

}
