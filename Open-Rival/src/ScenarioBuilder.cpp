#include "pch.h"
#include "ScenarioBuilder.h"

#include <iterator>
#include <memory>
#include <stdexcept>
#include <vector>

#include "SpriteComponent.h"

namespace Rival {

    ScenarioBuilder::ScenarioBuilder(ScenarioData data) : data(data) {}

    std::unique_ptr<Scenario> ScenarioBuilder::build(const EntityFactory& entityFactory) {

        // Initialize Tiles
        int numTiles = data.hdr.mapWidth * data.hdr.mapHeight;
        std::vector<Tile> tiles;
        tiles.reserve(numTiles);
        for (int i = 0; i < numTiles; ++i) {
            tiles.push_back(buildTile(data.tiles[i]));
        }

        std::unique_ptr<Scenario> scenario =
                std::make_unique<Scenario>(data.hdr.mapWidth, data.hdr.mapHeight, data.hdr.wilderness, tiles);

        // Initialize Units
        for (const UnitPlacement& unitPlacement : data.units) {
            addUnit(scenario.get(), unitPlacement, entityFactory);
        }

        // Initialize Buildings
        for (const BuildingPlacement& buildingPlacement : data.buildings) {
            if (buildingPlacement.type == 0xAB) {
                // Palisade (not supported yet)
                addPalisade(scenario.get(), buildingPlacement, entityFactory);
            } else if (buildingPlacement.type == 0xAC) {
                // Grate (not supported yet)
                continue;
            } else if (buildingPlacement.type == 0xAD) {
                // Door (not supported yet)
                continue;
            } else {
                // Player-owned building
                addBuilding(scenario.get(), buildingPlacement, entityFactory);
            }
        }

        // Initialize Objects
        for (const ObjectPlacement& objPlacement : data.objects) {
            addObject(scenario.get(), objPlacement, entityFactory);
        }

        return scenario;
    }

    /**
     * Gets the texture index of a terrain edge tile.
     *
     * @param tile TilePlacement whose texture index is needed.
     * @param baseIndex Texture index of the first tile of this edge type.
     * @param baseTileType Tile type of the first tile of this edge type.
     */
    std::uint8_t getTerrainEdgeTxIndex(TilePlacement& tile, std::uint8_t baseIndex, std::uint8_t baseTileType) {
        if (tile.type == baseTileType) {
            return baseIndex;
        } else if (tile.type == baseTileType + 1) {
            return baseIndex + 2;
        } else if (tile.type == baseTileType + 2) {
            return baseIndex + 4;
        } else if (tile.type == baseTileType + 3) {
            return baseIndex + 7;
        } else if (tile.type == baseTileType + 4) {
            return baseIndex + 9;
        } else if (tile.type == baseTileType + 5) {
            return baseIndex + 10;
        } else if (tile.type == baseTileType + 6) {
            return baseIndex + 13;
        } else if (tile.type == baseTileType + 7) {
            return baseIndex + 15;
        } else if (tile.type == baseTileType + 8) {
            return baseIndex + 17;
        } else if (tile.type == baseTileType + 9) {
            return baseIndex + 20;
        } else if (tile.type == baseTileType + 10) {
            return baseIndex + 21;
        } else if (tile.type == baseTileType + 11) {
            return baseIndex + 23;
        } else if (tile.type == baseTileType + 12) {
            return baseIndex + 26;
        } else {
            return baseIndex + 28;
        }
    }

    Tile ScenarioBuilder::buildTile(TilePlacement& tile) const {

        TileType type;
        std::uint8_t txIndex;

        if (tile.resource == 0) {

            if (tile.type == 0x00) {
                // Grass
                type = TileType::Grass;
                txIndex = 0 + tile.variant;

            } else if (tile.type >= 0x01 && tile.type <= 0x0e) {
                // Coastline
                type = TileType::Coastline;
                txIndex = getTerrainEdgeTxIndex(tile, 14, 0x01) + tile.variant;

            } else if (tile.type == 0x0f) {
                // Water
                type = TileType::Water;
                txIndex = 44 + tile.variant;

            } else if (tile.type >= 0x10 && tile.type <= 0x1d) {
                // Mud edge
                type = TileType::Mud;
                txIndex = getTerrainEdgeTxIndex(tile, 54, 0x10) + tile.variant;

            } else if (tile.type == 0x1e) {
                // Mud
                type = TileType::Mud;
                txIndex = 84 + tile.variant;

            } else if (tile.type >= 0x1f && tile.type <= 0x2c) {
                // Dirt edge
                type = TileType::Dirt;
                txIndex = getTerrainEdgeTxIndex(tile, 94, 0x1f) + tile.variant;

            } else if (tile.type == 0x2d) {
                // Dirt
                type = TileType::Dirt;
                txIndex = 124 + tile.variant;

            } else if (tile.type >= 0x2e && tile.type <= 0x3b) {
                // Dungeon edge
                type = TileType::Dungeon;
                txIndex = getTerrainEdgeTxIndex(tile, 134, 0x2e) + tile.variant;

            } else if (tile.type == 0x3c) {
                // Dungeon
                type = TileType::Dungeon;
                txIndex = 164 + tile.variant;

            } else {
                throw std::runtime_error("Unknown tile type: " + tile.type);
            }

        } else if (tile.resource == 1) {
            // Gold
            type = TileType::Gold;
            txIndex = 178;

        } else if (tile.resource == 2) {
            // Cropland
            type = TileType::Cropland;
            txIndex = 200;

        } else {
            throw std::runtime_error("Unknown tile resource: " + tile.resource);
        }

        return Tile(type, txIndex, 0);
    }

    void ScenarioBuilder::addUnit(
            Scenario* scenario, const UnitPlacement& unitPlacement, const EntityFactory& entityFactory) const {
        std::shared_ptr<Entity> unit = entityFactory.createUnit(unitPlacement);
        scenario->addEntity(unit, unitPlacement.x, unitPlacement.y);
    }

    void ScenarioBuilder::addPalisade(
            Scenario* scenario, const BuildingPlacement& buildingPlacement, const EntityFactory& entityFactory) const {
        std::shared_ptr<Entity> building = entityFactory.createPalisade(buildingPlacement, scenario->isWilderness());
        scenario->addEntity(building, buildingPlacement.x, buildingPlacement.y);
    }

    void ScenarioBuilder::addBuilding(
            Scenario* scenario, const BuildingPlacement& buildingPlacement, const EntityFactory& entityFactory) const {
        std::shared_ptr<Entity> building = entityFactory.createBuilding(buildingPlacement);
        scenario->addEntity(building, buildingPlacement.x, buildingPlacement.y);
    }

    void ScenarioBuilder::addObject(
            Scenario* scenario, const ObjectPlacement& objPlacement, const EntityFactory& entityFactory) const {
        std::shared_ptr<Entity> obj = entityFactory.createObject(objPlacement, scenario->isWilderness());
        scenario->addEntity(obj, objPlacement.x, objPlacement.y);
    }

}  // namespace Rival
