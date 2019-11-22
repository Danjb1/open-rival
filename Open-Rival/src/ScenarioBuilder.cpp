#include "pch.h"
#include "ScenarioBuilder.h"

#include <iterator>

namespace Rival {

    ScenarioBuilder::ScenarioBuilder(ScenarioData data) :
        data(data) {}

    std::unique_ptr<Scenario> ScenarioBuilder::build() {

        std::unique_ptr<Scenario> scenario = std::make_unique<Scenario>(
                data.hdr.mapWidth,
                data.hdr.mapHeight,
                data.hdr.wilderness);

        // Initialise Tiles
        std::vector<Tile>& tiles = scenario->getTiles();
        for (std::size_t i = 0; i != data.tiles.size(); i++) {
            tiles[i] = buildTile(data.tiles[i]);
        }

        // Add Units
        for (UnitPlacement& unitPlacement : data.units) {
            std::unique_ptr<Unit> unit = std::make_unique<Unit>(
                Unit::Type::LightCavalry,   // TODO: read type
                unitPlacement.x,
                unitPlacement.y);
            scenario->addUnit(std::move(unit));
        }

        return scenario;
    }

    std::uint8_t getCoastlineTxIndex(TilePlacement& tile) {
        if (tile.type == 0x01) {
            return 14;
        } else if (tile.type == 0x02) {
            return 16;
        } else if (tile.type == 0x03) {
            return 18;
        } else if (tile.type == 0x04) {
            return 21;
        } else if (tile.type == 0x05) {
            return 23;
        } else if (tile.type == 0x06) {
            return 24;
        } else if (tile.type == 0x07) {
            return 27;
        } else if (tile.type == 0x08) {
            return 29;
        } else if (tile.type == 0x09) {
            return 31;
        } else if (tile.type == 0x0a) {
            return 34;
        } else if (tile.type == 0x0b) {
            return 35;
        } else if (tile.type == 0x0c) {
            return 37;
        } else if (tile.type == 0x0d) {
            return 40;
        } else if (tile.type == 0x0e) {
            return 42;
        }
        return 0;
    }

    std::uint8_t getDungeonEdgeTxIndex(TilePlacement& tile) {
        if (tile.type == 0x2e) {
            return 120 + 14;
        } else if (tile.type == 0x2f) {
            return 120 + 16;
        } else if (tile.type == 0x30) {
            return 120 + 18;
        } else if (tile.type == 0x31) {
            return 120 + 21;
        } else if (tile.type == 0x32) {
            return 120 + 23;
        } else if (tile.type == 0x33) {
            return 120 + 24;
        } else if (tile.type == 0x34) {
            return 120 + 27;
        } else if (tile.type == 0x35) {
            return 120 + 29;
        } else if (tile.type == 0x36) {
            return 120 + 31;
        } else if (tile.type == 0x37) {
            return 120 + 34;
        } else if (tile.type == 0x38) {
            return 120 + 35;
        } else if (tile.type == 0x39) {
            return 120 + 37;
        } else if (tile.type == 0x3a) {
            return 120 + 40;
        } else if (tile.type == 0x3b) {
            return 120 + 42;
        }
        return 0;
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
                txIndex = getCoastlineTxIndex(tile) + tile.variant;

            } else if (tile.type == 0x0f) {
                // Water
                type = TileType::Water;
                txIndex = 45 + tile.variant;

            } else if (tile.type >= 0x10 && tile.type <= 0x1d) {
                // Mud edge
                type = TileType::Mud;
                txIndex = 84 + tile.variant;

            } else if (tile.type == 0x1e) {
                // Mud
                type = TileType::Mud;
                txIndex = 84 + tile.variant;

            } else if (tile.type >= 0x1f && tile.type <= 0x2c) {
                // Dirt edge
                type = TileType::Dirt;
                txIndex = 124 + tile.variant;

            } else if (tile.type == 0x2d) {
                // Dirt
                type = TileType::Dirt;
                txIndex = 124 + tile.variant;

            } else if (tile.type >= 0x2e && tile.type <= 0x3b) {
                // Dungeon edge
                type = TileType::Dungeon;
                txIndex = getDungeonEdgeTxIndex(tile) + tile.variant;

            } else if (tile.type == 0x3c) {
                // Dungeon
                type = TileType::Dungeon;
                txIndex = 164 + tile.variant;

            } else {
                throw std::runtime_error("Unknown tile type: "
                    + tile.type);
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
            throw std::runtime_error("Unknown tile resource: "
                + tile.resource);
        }

        return Tile(type, txIndex, 0);
    }

}
