#include "pch.h"
#include "Scenario.h"

namespace Rival {

    Scenario::Scenario(int width, int height, bool wilderness) :
            width(width),
            height(height),
            wilderness(wilderness),

            // Default to Grass everywhere
            tiles(std::vector<Tile>(
                    width * height, Tile(TileType::Grass, 0, 0))
            ),
            tilePassability(std::vector<TilePassability>(
                    width * height, TilePassability::Clear)
            ),
            nextBuildingId(0),
            nextId(0) {}

    int Scenario::getWidth() const {
        return width;
    }

    int Scenario::getHeight() const {
        return height;
    }

    const std::vector<Tile>& Scenario::getTiles() {
        return tiles;
    }

    void Scenario::tilesLoaded(std::vector<Tile> tiles) {
        this->tiles = tiles;
    }

    Tile Scenario::getTile(int x, int y) const {
        return tiles[y * width + x];
    }

    bool Scenario::isWilderness() const {
        return wilderness;
    }

    void Scenario::addBuilding(
            std::unique_ptr<Building> building,
            int player,
            int x,
            int y,
            uint8_t wallVariant) {

        // Add the Unit to the world
        buildings[nextBuildingId] = std::move(building);
        buildings[nextBuildingId]->addedToWorld(
                nextBuildingId,
                player,
                x,
                y,
                static_cast<WallVariant>(wallVariant));

        // Increase the ID for the next one
        nextBuildingId++;

        // Change the passability
        setPassability(x, y, TilePassability::Building);
    }

    void Scenario::addUnit(
            std::unique_ptr<Unit> unit,
            int player,
            int x,
            int y,
            Facing facing) {

        // Add the Unit to the world
        units[nextId] = std::move(unit);
        units[nextId]->addedToWorld(nextId, player, x, y, facing);

        // Increase the ID for the next one
        nextId++;

        // Change the passability
        setPassability(x, y, TilePassability::Unit);
    }

    void Scenario::setPassability(int x, int y, TilePassability passability) {
        tilePassability[y * width + x] = passability;
    }

    std::map<int, std::unique_ptr<Building>>& Scenario::getBuildings() {
        return buildings;
    }

    std::map<int, std::unique_ptr<Unit>>& Scenario::getUnits() {
        return units;
    }

}
