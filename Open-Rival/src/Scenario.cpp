#include "pch.h"
#include "Scenario.h"

namespace Rival {

    // Creates an empty Scenario
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

    // Creates a Scenario from data
    Scenario::Scenario(
            int width,
            int height,
            bool wilderness,
            std::vector<Tile> tiles)
        : width(width),
          height(height),
          wilderness(wilderness),
          tiles(tiles),
          tilePassability(std::vector<TilePassability>(
                  width * height, TilePassability::Clear)),
          nextId(0) {}

    Tile Scenario::getTile(int x, int y) const {
        return tiles[y * width + x];
    }

    void Scenario::addEntity(
            std::shared_ptr<Entity> entity,
            int x,
            int y) {

        // Add the Entity to the world
        entities[nextId] = entity;
        entities[nextId]->onSpawn(
                this,
                nextId,
                x,
                y);

        // Increase the ID for the next one
        ++nextId;
    }

    const std::vector<std::shared_ptr<Entity>> Scenario::getEntities() const {
        std::vector<std::shared_ptr<Entity>> entityList;
        for (auto const& entry : entities) {
            entityList.push_back(entry.second);
        }
        return entityList;
    }

    const std::shared_ptr<Entity> Scenario::getEntity(int id) const {
        auto const iter = entities.find(id);
        if (iter == entities.end()) {
            // Entity is not in the map
            return std::shared_ptr<Entity>();
        } else {
            // iter points to the desired map entry
            return iter->second;
        }
    }

    void Scenario::cleanUpEntities() {
        for (auto it = entities.cbegin(); it != entities.cend();) {
            const auto& e = it->second;
            if (e->isDeleted()) {
                it = entities.erase(it);
            } else {
                ++it;
            }
        }
    }

    void Scenario::setPassability(int x, int y, TilePassability passability) {
        tilePassability[y * width + x] = passability;
    }

}  // namespace Rival
