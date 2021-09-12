#ifndef SCENARIO_H
#define SCENARIO_H

#include <map>
#include <memory>
#include <vector>

#include "Entity.h"
#include "MapUtils.h"
#include "Tile.h"

// Forward-declarations
class Entity;

namespace Rival {

    /**
     * Interface exposing map data for pathfinding.
     */
    class PathfindingMap : public MapBounds {
    public:
        virtual TilePassability getPassability(const MapNode& pos) const = 0;
    };

    /**
     * A loaded scenario.
     *
     * Contains map data and entities.
     */
    class Scenario : public PathfindingMap {

    public:
        Scenario(int width, int height, bool wilderness);
        Scenario(int width, int height, bool wilderness, std::vector<Tile> tiles);

        // Begin PathfindingMap override
        int getWidth() const override { return width; }
        int getHeight() const override { return height; }
        TilePassability getPassability(const MapNode& pos) const override;
        // End PathfindingMap override

        const std::vector<Tile>& getTiles() const { return tiles; }

        Tile getTile(int x, int y) const;

        bool isWilderness() const { return wilderness; }

        void addEntity(
                std::shared_ptr<Entity> unit,
                int x,
                int y);

        /**
         * Returns a read-only list of all entities currently present in the
         * world.
         */
        const std::vector<std::shared_ptr<Entity>> getEntities() const;

        /**
         * Gets an entity by its ID.
         *
         * If the entity is not found, an empty shared_ptr is returned.
         */
        const std::shared_ptr<Entity> getEntity(int id) const;

        void cleanUpEntities();

        void setPassability(const MapNode& pos, TilePassability passability);

    private:
        const int width;
        const int height;
        bool wilderness;
        std::vector<Tile> tiles;
        std::vector<TilePassability> tilePassability;

        int nextId;
        std::map<int, std::shared_ptr<Entity>> entities;
    };

}  // namespace Rival

#endif  // SCENARIO_H
