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

    /**
     *
     */
    std::uint8_t getTerrainEdgeTxIndex(
            TilePlacement& tile,
            std::uint8_t baseIndex,
            std::uint8_t baseTileType) {
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
