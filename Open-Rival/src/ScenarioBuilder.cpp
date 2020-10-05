#include "pch.h"
#include "ScenarioBuilder.h"

#include <iterator>
#include <memory>
#include <stdexcept>
#include <vector>

#include "SpriteComponent.h"

namespace Rival {

    ScenarioBuilder::ScenarioBuilder(ScenarioData data)
        : data(data) {}

    std::unique_ptr<Scenario> ScenarioBuilder::build(const Resources& res) {

        std::unique_ptr<Scenario> scenario = std::make_unique<Scenario>(
                data.hdr.mapWidth,
                data.hdr.mapHeight,
                data.hdr.wilderness);

        // Initialise Tiles
        int numTiles = data.hdr.mapWidth * data.hdr.mapHeight;
        std::vector<Tile> tiles;
        tiles.reserve(numTiles);
        for (int i = 0; i < numTiles; ++i) {
            tiles.push_back(buildTile(data.tiles[i]));
        }
        scenario->tilesLoaded(tiles);

        // Initialise Units
        for (UnitPlacement& unitPlacement : data.units) {

            // Create Unit
            std::unique_ptr<Entity> unit = std::make_unique<Entity>(
                    Unit::width, Unit::height);

            // Add components
            const Unit::Type unitType = getUnitType(unitPlacement.type);
            const Spritesheet& spritesheet = res.getUnitSpritesheet(unitType);
            unit->attach(std::make_unique<SpriteComponent>(spritesheet));
            // TODO: add facing: getFacing(unitPlacement.facing)
            // TODO: add type: getUnitType(unitPlacement.type))
            // TODO: set player: unitPlacement.player

            // Add to world
            scenario->addEntity(std::move(unit),
                    unitPlacement.x,
                    unitPlacement.y);
        }

        // Initialise Buildings
        for (BuildingPlacement& buildingPlacement : data.buildings) {
            Building::Type buildingType =
                    getBuildingType(buildingPlacement.type);
            int width = Building::getWidth(buildingType);
            int height = Building::getHeight(buildingType);
            std::unique_ptr<Entity> building = std::make_unique<Entity>(
                    width, height);
            // TODO: set player: unitPlacement.player
            // TODO: add type: getBuilding::Type(buildingPlacement.type))
            // TODO: set wall variant: buildingPlacement.wallVariant
            scenario->addEntity(std::move(building),
                    buildingPlacement.x,
                    buildingPlacement.y);
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

    Building::Type ScenarioBuilder::getBuildingType(
            std::uint8_t buildingType) const {
        switch (buildingType) {
        case 0x27:
            return Building::Type::ElvenKeep;
        case 0x28:
            return Building::Type::Treasury;
        case 0x29:
            return Building::Type::CombatCamp;
        case 0x2a:
            return Building::Type::Arsenal;
        case 0x2b:
            return Building::Type::DuelRange;
        case 0x2c:
            return Building::Type::HolyNest;
        case 0x2d:
            return Building::Type::MinerGuildhall;
        case 0x2e:
            return Building::Type::AbbeyTower;
        case 0x2f:
            return Building::Type::CouncilOfRunes;
        case 0x30:
            return Building::Type::Harbour;
        case 0x31:
            return Building::Type::WarningTower;
        case 0x32:
            return Building::Type::TreeWall;
        case 0x1b:
            return Building::Type::Fortress;
        case 0x1c:
            return Building::Type::HoardKeep;
        case 0x1d:
            return Building::Type::Fort;
        case 0x1e:
            return Building::Type::Blacksmith;
        case 0x1f:
            return Building::Type::BattleQuarters;
        case 0x20:
            return Building::Type::BlackNest;
        case 0x21:
            return Building::Type::WeirdWorkshop;
        case 0x22:
            return Building::Type::UnholyChapel;
        case 0x23:
            return Building::Type::AltarOfDoom;
        case 0x24:
            return Building::Type::Docks;
        case 0x25:
            return Building::Type::GuardTower;
        case 0x26:
            return Building::Type::GreenskinWall;
        case 0x0f:
            return Building::Type::Castle;
        case 0x10:
            return Building::Type::GoldMill;
        case 0x11:
            return Building::Type::ArcheryRange;
        case 0x12:
            return Building::Type::Armoury;
        case 0x13:
            return Building::Type::Barracks;
        case 0x14:
            return Building::Type::HolyStables;
        case 0x15:
            return Building::Type::FireGuild;
        case 0x16:
            return Building::Type::Temple;
        case 0x17:
            return Building::Type::MageTower;
        case 0x18:
            return Building::Type::Shipyard;
        case 0x19:
            return Building::Type::WatchTower;
        case 0x1a:
            return Building::Type::Wall;
        default:
            throw std::runtime_error("Unknown building type: " + unsigned(buildingType));
        }
    }

    Unit::Type ScenarioBuilder::getUnitType(std::uint8_t unitType) const {
        switch (unitType) {
        case 0x33:
            return Unit::Type::Peasant;
        case 0x34:
            return Unit::Type::Bowman;
        case 0x35:
            return Unit::Type::LightCavalry;
        case 0x36:
            return Unit::Type::Knight;
        case 0x37:
            return Unit::Type::FireMaster;
        case 0x38:
            return Unit::Type::Thief;
        case 0x39:
            return Unit::Type::Ballista;
        case 0x3A:
            return Unit::Type::ChariotOfWar;
        case 0x3B:
            return Unit::Type::Wizard;
        case 0x3C:
            return Unit::Type::Priest;
        case 0x3D:
            return Unit::Type::SeaBarge;
        case 0x3E:
            return Unit::Type::Battleship;
        case 0x3F:
            return Unit::Type::PegasRider;
        case 0x40:
            return Unit::Type::Zeppelin;
        case 0x41:
            return Unit::Type::Serf;
        case 0x42:
            return Unit::Type::RockThrower;
        case 0x43:
            return Unit::Type::HordeRider;
        case 0x44:
            return Unit::Type::Warlord;
        case 0x45:
            return Unit::Type::GnomeBoomer;
        case 0x46:
            return Unit::Type::Rogue;
        case 0x47:
            return Unit::Type::Catapult;
        case 0x48:
            return Unit::Type::StormTrooper;
        case 0x49:
            return Unit::Type::PriestOfDoom;
        case 0x4A:
            return Unit::Type::Necromancer;
        case 0x4B:
            return Unit::Type::LandingCraft;
        case 0x4C:
            return Unit::Type::TrollGalley;
        case 0x4D:
            return Unit::Type::Warbat;
        case 0x4E:
            return Unit::Type::Balloon;
        case 0x4F:
            return Unit::Type::Yeoman;
        case 0x50:
            return Unit::Type::Archer;
        case 0x51:
            return Unit::Type::Druid;
        case 0x52:
            return Unit::Type::Centaur;
        case 0x53:
            return Unit::Type::DwarfMiner;
        case 0x54:
            return Unit::Type::Scout;
        case 0x55:
            return Unit::Type::Bombard;
        case 0x56:
            return Unit::Type::Arquebusier;
        case 0x57:
            return Unit::Type::Mage;
        case 0x58:
            return Unit::Type::Enchanter;
        case 0x59:
            return Unit::Type::Bark;
        case 0x60:
            return Unit::Type::Warship;
        case 0x5B:
            return Unit::Type::SkyRider;
        case 0x5C:
            return Unit::Type::MagicChopper;
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

}  // namespace Rival
