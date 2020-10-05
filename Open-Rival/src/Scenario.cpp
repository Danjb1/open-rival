#include "pch.h"
#include "Scenario.h"

namespace Rival {

    Scenario::Scenario(int width, int height, bool wilderness)
        : width(width),
          height(height),
          wilderness(wilderness),

          // Default to Grass everywhere
          tiles(std::vector<Tile>(
                  width * height, Tile(TileType::Grass, 0, 0))),
          tilePassability(std::vector<TilePassability>(
                  width * height, TilePassability::Clear)),
          nextId(0) {}

    int Scenario::getWidth() const {
        return width;
    }

    int Scenario::getHeight() const {
        return height;
    }

    const std::vector<Tile>& Scenario::getTiles() const {
        return tiles;
    }

    void Scenario::tilesLoaded(std::vector<Tile> newTiles) {
        tiles = newTiles;
    }

    Tile Scenario::getTile(int x, int y) const {
        return tiles[y * width + x];
    }

    bool Scenario::isWilderness() const {
        return wilderness;
    }

    void Scenario::addEntity(
            std::unique_ptr<Entity> entity,
            int x,
            int y) {

        // Add the Entity to the world
        entities[nextId] = std::move(entity);
        entities[nextId]->onSpawn(
                nextId,
                x,
                y);

        // Increase the ID for the next one
        ++nextId;

        // Change the passability
        setPassability(x, y, TilePassability::Building);
    }

    void Scenario::setPassability(int x, int y, TilePassability passability) {
        tilePassability[y * width + x] = passability;
    }

    const std::map<int, std::unique_ptr<Entity>>& Scenario::getEntities() const {
        return entities;
    }

}  // namespace Rival
