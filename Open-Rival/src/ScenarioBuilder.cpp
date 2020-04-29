#include "pch.h"
#include "ScenarioBuilder.h"

#include <iterator>
#include <stdexcept>
#include <vector>

namespace Rival {

    ScenarioBuilder::ScenarioBuilder(ScenarioData data) :
        data(data) {}

    std::shared_ptr<Scenario> ScenarioBuilder::build() {

        std::shared_ptr<Scenario> scenario = std::make_shared<Scenario>(
                data.hdr.mapWidth,
                data.hdr.mapHeight,
                data.hdr.wilderness);

        // Initialise Tiles
        int numTiles = data.hdr.mapWidth * data.hdr.mapHeight;
        std::vector<Tile> tiles;
        tiles.reserve(numTiles);
        for (int i = 0; i < numTiles; i++) {
            tiles.push_back(buildTile(data.tiles[i]));
        }
        scenario->tilesLoaded(tiles);

        // Initialise Units
        for (UnitPlacement& unitPlacement : data.units) {
            std::unique_ptr<Unit> unit = std::make_unique<Unit>(
                    getUnitType(unitPlacement.type));
            scenario->addUnit(std::move(unit),
                    unitPlacement.player,
                    unitPlacement.x,
                    unitPlacement.y,
                    getFacing(unitPlacement.facing));
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

    UnitType ScenarioBuilder::getUnitType(std::uint8_t unitType) const {
        switch (unitType) {
        case 0x33:
            return UnitType::Peasant;
        case 0x34:
            return UnitType::Bowman;
        case 0x35:
            return UnitType::LightCavalry;
        case 0x36:
            return UnitType::Knight;
        case 0x37:
            return UnitType::FireMaster;
        case 0x38:
            return UnitType::Thief;
        case 0x39:
            return UnitType::Ballista;
        case 0x3A:
            return UnitType::ChariotOfWar;
        case 0x3B:
            return UnitType::Wizard;
        case 0x3C:
            return UnitType::Priest;
        case 0x3D:
            return UnitType::SeaBarge;
        case 0x3E:
            return UnitType::Battleship;
        case 0x3F:
            return UnitType::PegasRider;
        case 0x40:
            return UnitType::Zeppelin;
        case 0x41:
            return UnitType::Serf;
        case 0x42:
            return UnitType::RockThrower;
        case 0x43:
            return UnitType::HordeRider;
        case 0x44:
            return UnitType::Warlord;
        case 0x45:
            return UnitType::GnomeBoomer;
        case 0x46:
            return UnitType::Rogue;
        case 0x47:
            return UnitType::Catapult;
        case 0x48:
            return UnitType::StormTrooper;
        case 0x49:
            return UnitType::PriestOfDoom;
        case 0x4A:
            return UnitType::Necromancer;
        case 0x4B:
            return UnitType::LandingCraft;
        case 0x4C:
            return UnitType::TrollGalley;
        case 0x4D:
            return UnitType::Warbat;
        case 0x4E:
            return UnitType::Balloon;
        case 0x4F:
            return UnitType::Yeoman;
        case 0x50:
            return UnitType::Archer;
        case 0x51:
            return UnitType::Druid;
        case 0x52:
            return UnitType::Centaur;
        case 0x53:
            return UnitType::DwarfMiner;
        case 0x54:
            return UnitType::Scout;
        case 0x55:
            return UnitType::Bombard;
        case 0x56:
            return UnitType::Arquebusier;
        case 0x57:
            return UnitType::Mage;
        case 0x58:
            return UnitType::Enchanter;
        case 0x59:
            return UnitType::Bark;
        case 0x60:
            return UnitType::Warship;
        case 0x5B:
            return UnitType::SkyRider;
        case 0x5C:
            return UnitType::MagicChopper;
        default:
            throw std::runtime_error("Unknown unit type: " + unitType);
        }
    }

    Facing ScenarioBuilder::getFacing(std::uint8_t facing) const {
        switch (facing) {
        case 0:
            return Facing::South;
        case 1:
            return Facing::SouthWest;
        case 2:
            return Facing::West;
        case 3:
            return Facing::NorthWest;
        case 4:
            return Facing::North;
        case 5:
            return Facing::NorthEast;
        case 6:
            return Facing::East;
        case 7:
            return Facing::SouthEast;
        default:
            throw std::runtime_error("Unknown facing: " + facing);
        }
    }

}
