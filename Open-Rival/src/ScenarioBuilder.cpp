#include "pch.h"
#include "ScenarioBuilder.h"

#include <iterator>
#include <stdexcept>

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

        // Initialise Units
        for (UnitPlacement& unitPlacement : data.units) {
            std::unique_ptr<Unit> unit = std::make_unique<Unit>(
                getUnitType(unitPlacement.type),
                unitPlacement.x,
                unitPlacement.y);
            scenario->addUnit(std::move(unit));
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

    Unit::Type ScenarioBuilder::getUnitType(std::uint8_t unitType) const {
        switch (unitType) {
        case 0x33:
            return Unit::Peasant;
        case 0x34:
            return Unit::Bowman;
        case 0x35:
            return Unit::LightCavalry;
        case 0x36:
            return Unit::Knight;
        case 0x37:
            return Unit::FireMaster;
        case 0x38:
            return Unit::Thief;
        case 0x39:
            return Unit::Ballista;
        case 0x3A:
            return Unit::ChariotOfWar;
        case 0x3B:
            return Unit::Wizard;
        case 0x3C:
            return Unit::Priest;
        case 0x3D:
            return Unit::SeaBarge;
        case 0x3E:
            return Unit::Battleship;
        case 0x3F:
            return Unit::PegasRider;
        case 0x40:
            return Unit::Zeppelin;
        case 0x41:
            return Unit::Serf;
        case 0x42:
            return Unit::RockThrower;
        case 0x43:
            return Unit::HordeRider;
        case 0x44:
            return Unit::Warlord;
        case 0x45:
            return Unit::GnomeBoomer;
        case 0x46:
            return Unit::Rogue;
        case 0x47:
            return Unit::Catapult;
        case 0x48:
            return Unit::StormTrooper;
        case 0x49:
            return Unit::PriestOfDoom;
        case 0x4A:
            return Unit::Necromancer;
        case 0x4B:
            return Unit::LandingCraft;
        case 0x4C:
            return Unit::TrollGalley;
        case 0x4D:
            return Unit::Warbat;
        case 0x4E:
            return Unit::Balloon;
        case 0x4F:
            return Unit::Yeoman;
        case 0x50:
            return Unit::Archer;
        case 0x51:
            return Unit::Druid;
        case 0x52:
            return Unit::Centaur;
        case 0x53:
            return Unit::DwarfMiner;
        case 0x54:
            return Unit::Scout;
        case 0x55:
            return Unit::Bombard;
        case 0x56:
            return Unit::Arquebusier;
        case 0x57:
            return Unit::Mage;
        case 0x58:
            return Unit::Enchanter;
        case 0x59:
            return Unit::Bark;
        case 0x60:
            return Unit::Warship;
        case 0x5B:
            return Unit::SkyRider;
        case 0x5C:
            return Unit::MagicChopper;
        default:
            throw std::runtime_error("Unknown unit type: " + unitType);
        }
    }

}
