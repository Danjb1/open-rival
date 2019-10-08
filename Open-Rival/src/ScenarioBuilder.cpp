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
            // TODO: determine txIndex
            tiles[i] = Tile(getTileType(data.tiles[i]), 0, 0);
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

    TileType ScenarioBuilder::getTileType(TilePlacement tile) {

        if (tile.resource == 0) {

            if (tile.type == 0x00) {
                // Grass
                return TileType::Grass;

            } else if (tile.type >= 0x01 && tile.type <= 0x0e) {
                // Coastline
                return TileType::Coastline;

            } else if (tile.type == 0x0f) {
                // Water
                return TileType::Water;

            } else if (tile.type >= 0x10 && tile.type <= 0x1d) {
                // Mud edge
                return TileType::Mud;

            } else if (tile.type == 0x1e) {
                // Mud
                return TileType::Mud;

            } else if (tile.type >= 0x1f && tile.type <= 0x2c) {
                // Dirt edge
                return TileType::Dirt;

            } else if (tile.type == 0x2d) {
                // Dirt
                return TileType::Dirt;

            } else if (tile.type >= 0x2e && tile.type <= 0x3c) {
                // Dungeon edge
                return TileType::Dungeon;

            } else if (tile.type == 0x3d) {
                // Dungeon
                return TileType::Dungeon;

            } else {
                throw std::runtime_error("Unknown tile type: "
                    + tile.type);
            }

        } else if (tile.resource == 1) {
            // Gold
            return TileType::Gold;

        } else if (tile.resource == 2) {
            // Cropland
            return TileType::Cropland;

        }

        throw std::runtime_error("Unknown tile resource: "
            + tile.resource);
    }

}
