#include "pch.h"
#include "ScenarioReader.h"

#include <fstream>
#include <iomanip>
#include <iostream>

namespace Rival {

    ScenarioReader::ScenarioReader(const std::string filename) {

        // Open the file at the end
        std::ifstream is(filename, std::ios::binary|std::ios::ate);
        if (!is.is_open()) {
            throw std::runtime_error("Failed to open scenario: " + filename);
        }

        // Create a buffer to hold the entire file contents
        std::streampos size = is.tellg();
        if (size == -1) {
            throw std::runtime_error("Failed to retrieve file size");
        }
        std::vector<unsigned char> data(static_cast<size_t>(size));

        // Read the entire file to memory
        is.seekg(0, std::ios::beg);
        is.read(reinterpret_cast<char*>(data.data()), size);
        is.close();

        ScenarioFile scenarioFile;

        // Parse map header
        printSection("Parsing header");
        printOffset();
        scenarioFile.hdr = parseHeader(data);
        print(scenarioFile.hdr);

        // Parse player properties
        printSection("Parsing player properties");
        printOffset();
        for (int i = 0; i < 8; i++) {
            scenarioFile.playerProperties[i] = parsePlayerProperties(data);
        }
        print(scenarioFile.playerProperties[0]);

        // UNKNOWN
        printSection("Skipping unknown section");
        printOffset();
        skip(data, 909, true);

        // Parse troop defaults
        printSection("Parsing troop defaults");
        printOffset();
        for (int i = 0; i < numTroops; i++) {
            scenarioFile.troopDefaults[i] = parseTroopDefaults(data);
        }
        print(scenarioFile.troopDefaults[0]);
        skip(data, 7, true);

        // Parse upgrade properties
        printSection("Parsing upgrade properties");
        printOffset();
        for (int i = 0; i < numUpgrades; i++) {
            scenarioFile.upgradeProperties[i] =
                    parseUpgradeProperties(data, doesUpgradeHaveAmount(i));
        }
        print(scenarioFile.upgradeProperties[0]);

        // UNKNOWN
        printSection("Skipping unknown section");
        printOffset();
        skip(data, 176, true);

        // Parse unit production costs
        printSection("Parsing unit production costs");
        printOffset();
        for (int i = 0; i < numProductionCosts; i++) {
            scenarioFile.productionCosts[i] = parseProductionCost(data);
        }
        print(scenarioFile.productionCosts[0]);
        skip(data, 27, true);

        // Parse weapon defaults
        printSection("Parsing weapon defaults");
        printOffset();
        for (int i = 0; i < numWeapons; i++) {
            scenarioFile.weaponDefaults[i] = parseWeaponDefaults(data);
        }
        print(scenarioFile.weaponDefaults[0]);
        skip(data, 2, true);

        // Parse available buildings
        printSection("Parsing available buildings");
        printOffset();
        scenarioFile.availableBuildings = parseAvailableBuildings(data);
        print(scenarioFile.availableBuildings);
        skip(data, 5, true);

        // Parse monster defaults
        printSection("Parsing monster defaults");
        printOffset();
        for (int i = 0; i < numMonsters; i++) {
            scenarioFile.monsterDefaults[i] = parseTroopDefaults(data);
        }
        print(scenarioFile.monsterDefaults[0]);
        skip(data, 49, true);

        // Parse hire troops restrictions
        printSection("Parsing hire troops restrictions");
        printOffset();
        HireTroopsRestrictions restrictions =
                parseHireTroopsRestrictions(data);
        print(restrictions);

        // Parse AI building settings
        printSection("Parsing AI building settings");
        printOffset();
        skip(data, 168, true); // for now

        // Parse AI troop settings
        printSection("Parsing AI troop settings");
        printOffset();
        skip(data, 280, true); // for now

        // UNKNOWN
        printSection("Skipping unknown section");
        printOffset();
        skip(data, 308, true);

        // Parse terrain data
        printSection("Parsing terrain data");
        printOffset();
        scenarioFile.tiles = parseTerrain(
                data, scenarioFile.hdr.mapWidth, scenarioFile.hdr.mapHeight);
        skip(data, 4, true);

        // Parse buildings
        printSection("Parsing buildings");
        printOffset();
        printNext(data, 128);
        scenarioFile.buildings = parseBuildings(data);
        print(scenarioFile.buildings[0]);

        // Parse units
        printSection("Parsing units");
        printOffset();
        skip(data, 128, true); // size unknown

        // Parse traps
        printSection("Parsing traps");
        printOffset();

        // Parse scenario goals
        printSection("Parsing scenario goals");
        printOffset();

        // Parse campaign texts
        printSection("Parsing campaign texts");
        printOffset();

        // Parse AI building settings
        printSection("Parsing AI building settings");
        printOffset();

        // Parse AI troop settings
        printSection("Parsing AI troop settings");
        printOffset();

        // Parse objects
        printSection("Parsing objects");
        printOffset();

        // Parse checksum
        printSection("Parsing checksum");
        printOffset();
    }

    ///////////////////////////////////////////////////////////////////////////
    // Parsing
    ///////////////////////////////////////////////////////////////////////////

    ScenarioHeader ScenarioReader::parseHeader(
            std::vector<unsigned char>& data) {

        ScenarioHeader hdr;

        hdr.unknown1 = readInt(data);
        hdr.unknown2 = readInt(data);
        std::uint8_t terrainType = readByte(data);
        hdr.wilderness = (terrainType == 0x03);
        std::uint8_t nameLength = readByte(data);
        hdr.mapName = readString(data, nameLength);
        hdr.mapHeight = readInt(data);
        hdr.mapWidth = readInt(data);
        skip(data, 9, false);

        return hdr;
    }

    PlayerProperties ScenarioReader::parsePlayerProperties(
            std::vector<unsigned char>& data) {

        PlayerProperties props;

        skip(data, 12, false);
        props.startingFood = readInt(data);
        props.startingWood = readInt(data);
        props.startingGold = readInt(data);
        props.race = readByte(data);
        props.ai = readBool(data);
        props.aiType = readByte(data);
        props.aiPerformance = readByte(data);
        props.aiStrategy = readByte(data);

        return props;
    }

    TroopDefaults ScenarioReader::parseTroopDefaults(
            std::vector<unsigned char>& data) {

        TroopDefaults troop;

        troop.hitpoints = readShort(data);
        troop.magic = readShort(data);
        troop.armour = readByte(data);
        skip(data, 2, false);
        troop.sight = readByte(data);
        troop.range = readByte(data);
        skip(data, 39, false);

        return troop;
    }

    bool ScenarioReader::doesUpgradeHaveAmount(int i) const {
        // Upgrades 0 - 47 and 67 - 69 do not have an "Amount" field
        return i <= 47 || (i >= 67 && i <= 69);
    }

    UpgradeProperties ScenarioReader::parseUpgradeProperties(
            std::vector<unsigned char>& data, bool readAmount) {

        UpgradeProperties upgrade;

        if (readAmount) {
            upgrade.amount = readInt(data);
        } else {
            upgrade.amount = 0;
            skip(data, 4, false);
        }
        upgrade.goldCost = readInt(data);
        upgrade.woodCost = readInt(data);
        upgrade.unknown = readInt(data);

        return upgrade;
    }

    ProductionCost ScenarioReader::parseProductionCost(
            std::vector<unsigned char>& data) {

        ProductionCost cost;

        cost.goldCost = readShort(data);
        cost.woodCost = readShort(data);
        cost.constructionTime = readInt(data);
        cost.requiredExpOrIncreasePercent = readInt(data);

        return cost;
    }

    WeaponDefaults ScenarioReader::parseWeaponDefaults(
            std::vector<unsigned char>& data) {

        WeaponDefaults weapon;

        weapon.moveSpaces = readShort(data);
        weapon.moveTime = readShort(data);
        weapon.damage = readShort(data);
        weapon.penetrate = readShort(data);
        weapon.accuracy = readShort(data);
        weapon.effectRange = readByte(data);
        weapon.attackRange = readByte(data);
        weapon.manaCost = readShort(data);
        weapon.reloadTime = readInt(data);
        weapon.unknown = readShort(data);
        skip(data, 1, false);

        return weapon;
    }

    AvailableBuildings ScenarioReader::parseAvailableBuildings(
        std::vector<unsigned char>& data) {

        AvailableBuildings bldgs;

        bldgs.cropLand = readBool(data);
        bldgs.goldAmplifier = readBool(data);
        bldgs.rangedTroopBuilding = readBool(data);
        bldgs.siegeTroopBuilding = readBool(data);
        bldgs.meleeTroopBuilding = readBool(data);
        bldgs.flyingTroopBuilding = readBool(data);
        bldgs.engineerTroopBuilding = readBool(data);
        bldgs.healerTroopBuilding = readBool(data);
        bldgs.spellcasterTroopBuilding = readBool(data);
        bldgs.shipyard = readBool(data);
        bldgs.watchTower = readBool(data);
        bldgs.wall = readBool(data);

        return bldgs;
    }

    HireTroopsRestrictions ScenarioReader::parseHireTroopsRestrictions(
        std::vector<unsigned char>& data) {

        HireTroopsRestrictions restrictions;

        restrictions.worker = readBool(data);
        restrictions.rangedTroop = readBool(data);
        restrictions.lightMeleeOrSpellcasterTroop = readBool(data);
        restrictions.heavyMeleeTroop = readBool(data);
        restrictions.engineer = readBool(data);
        restrictions.stealthTroop = readBool(data);
        restrictions.siegeTroop = readBool(data);
        restrictions.raceBonusTroop = readBool(data);
        restrictions.spellcaster = readBool(data);
        restrictions.healer = readBool(data);
        restrictions.transportShip = readBool(data);
        restrictions.combatShip = readBool(data);
        restrictions.flyingTroop = readBool(data);
        restrictions.flyingTransport = readBool(data);
        restrictions.mustHire = readBool(data);

        return restrictions;
    }

    std::vector<TilePlacement> ScenarioReader::parseTerrain(
            std::vector<unsigned char>& data,
            int width,
            int height) {

        unsigned int numTiles = width * height;
        std::vector<TilePlacement> tiles(numTiles);
        size_t tileId = 0;

        while (tileId < numTiles) {

            // Determine how many tiles the next tile definition describes,
            // and skip to the tile definition
            bool readMultiple = (readByte(data) == 0xfa);
            std::uint16_t numTiles = 1;
            if (readMultiple) {
                numTiles = readShort(data);
                skip(data, 4, false);
            } else {
                skip(data, 3, false);
            }

            // Read the tile definition
            TilePlacement tile = parseTile(data);

            // Copy the tile definition to the number of tiles it describes
            for (size_t i = 0; i < numTiles; i++) {
                tiles.push_back(tile);
                tileId++;
            }

            pos += bytesPerTile;
        }

        return tiles;
    }

    TilePlacement ScenarioReader::parseTile(
            std::vector<unsigned char>& data) {

        TilePlacement tile;
        std::uint8_t resource = readByte(data, pos);

        if (resource == 0) {

            std::uint8_t type = readByte(data, pos + 2);

            if (type == 0x00) {
                // Grass
                tile.type = TileType::Grass;

            } else if (type >= 0x01 && type <= 0x0e) {
                // Coastline
                tile.type = TileType::Coastline;

            } else if (type == 0x0f) {
                // Water
                tile.type = TileType::Water;

            } else if (type >= 0x10 && type <= 0x1d) {
                // Mud edge
                tile.type = TileType::Mud;

            } else if (type == 0x1e) {
                // Mud
                tile.type = TileType::Mud;

            } else if (type >= 0x1f && type <= 0x2c) {
                // Dirt edge
                tile.type = TileType::Dirt;

            } else if (type == 0x2d) {
                // Dirt
                tile.type = TileType::Dirt;

            } else if (type >= 0x2e && type <= 0x3c) {
                // Dungeon edge
                tile.type = TileType::Dungeon;

            } else if (type == 0x3d) {
                // Dungeon
                tile.type = TileType::Dungeon;

            } else {
                std::cout << "Unknown tile: ";
                printNext(data, 6);
            }

        } else if (resource == 1) {
            // Gold
            tile.type = TileType::Gold;

        } else if (resource == 2) {
            // Cropland
            tile.type = TileType::Cropland;

        } else {
            std::cout << "Unknown tile: ";
            printNext(data, 6);
        }

        return tile;
    }

    std::vector<BuildingPlacement> ScenarioReader::parseBuildings(
        std::vector<unsigned char>& data) {

        std::uint32_t numBuildings = readInt(data);
        std::vector<BuildingPlacement> buildings;
        buildings.reserve(numBuildings);

        for (unsigned int i = 0; i < numBuildings; i++) {
            BuildingPlacement bldg = parseBuilding(data);
            buildings.push_back(bldg);
        }

        return buildings;
    }

    BuildingPlacement ScenarioReader::parseBuilding(
            std::vector<unsigned char>& data) {

        BuildingPlacement bldg;

        bldg.type = readByte(data);
        bldg.player = readByte(data);
        bldg.x = readShort(data);
        bldg.y = readShort(data);
        bldg.hitpoints = readShort(data);
        bldg.armour = readShort(data);
        skip(data, 1, false);
        bldg.sight = readByte(data);
        bldg.range = readByte(data);
        bldg.upgrade1Enabled = readBool(data);
        bldg.upgrade2Enabled = readBool(data);
        bldg.specialColour = readByte(data);
        bldg.prisoner = readBool(data);
        bldg.name = readString(data, 12);

        return bldg;
    }

    UnitPlacement ScenarioReader::parseUnit(std::vector<unsigned char>& data) {

        UnitPlacement unit;

        unit.type = readByte(data);
        skip(data, 2, false);
        unit.facing = readByte(data);
        skip(data, 1, false);
        unit.x = readShort(data);
        unit.y = readShort(data);
        unit.player = readByte(data);
        unit.hitpoints = readShort(data);
        unit.magic = readByte(data);
        unit.armour = readShort(data);
        skip(data, 1, false);
        unit.type2 = readByte(data);
        unit.sight = readByte(data);
        unit.range = readByte(data);
        skip(data, 2, false);
        unit.specialColour = readByte(data);
        unit.prisoner = readBool(data);
        unit.goldCost = readShort(data);
        unit.woodCost = readShort(data);
        unit.name = readString(data, 12);
        skip(data, 13, false);
        unit.upgrade1Enabled = readBool(data);
        unit.upgrade2Enabled = readBool(data);
        unit.upgrade3Enabled = readBool(data);
        unit.upgrade4Enabled = readBool(data);
        unit.fightingArea = readByte(data);

        return unit;
    }

    TrapPlacement ScenarioReader::parseTrap(std::vector<unsigned char>& data) {

        TrapPlacement trap;

        trap.x = readByte(data);
        trap.y = readByte(data);
        trap.player = readByte(data);

        return trap;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Token Extraction
    ///////////////////////////////////////////////////////////////////////////

    std::uint8_t ScenarioReader::readByte(
            std::vector<unsigned char>& data) {
        std::uint8_t value = readByte(data, pos);
        pos += 1;
        return value;
    }

    std::uint8_t ScenarioReader::readByte(
            std::vector<unsigned char>& data, size_t offset) const {
        return std::uint8_t(data[offset + 0]);
    }

    bool ScenarioReader::readBool(std::vector<unsigned char>& data) {
        bool value = readBool(data, pos);
        pos += 1;
        return value;
    }

    bool ScenarioReader::readBool(
            std::vector<unsigned char>& data, size_t offset) const {
        uint8_t value = data[offset];
        return value == 1;
    }

    std::uint16_t ScenarioReader::readShort(
        std::vector<unsigned char>& data) {
        std::uint16_t value = readShort(data, pos);
        pos += numBytesShort;
        return value;
    }

    uint16_t ScenarioReader::readShort(
        std::vector<unsigned char>& data, size_t offset) const {
        // little endian
        return std::uint16_t(
            data[offset + 1] << 8 |
            data[offset + 0]
        );
    }

    std::uint32_t ScenarioReader::readInt(
             std::vector<unsigned char>& data) {
        std::uint32_t value = readInt(data, pos);
        pos += numBytesInt;
        return value;
    }

    uint32_t ScenarioReader::readInt(
            std::vector<unsigned char>& data, size_t offset) const {
        // little endian
        return std::uint32_t(
            data[offset + 3] << 24 |
            data[offset + 2] << 16 |
            data[offset + 1] << 8 |
            data[offset + 0]
        );
    }

    std::string ScenarioReader::readString(
            std::vector<unsigned char>& data, size_t length) {
        std::string value = readString(data, pos, length);
        pos += length;
        return value;
    }

    std::string ScenarioReader::readString(
            std::vector<unsigned char>& data,
            size_t offset,
            size_t length) const {

        std::vector<char> nameChars(length);
        for (size_t i = 0; i < length; i++) {
            nameChars[i] = data[offset + i];
        }
        std::string value(nameChars.data(), length);
        return value;
    }

    void ScenarioReader::skip(
            std::vector<unsigned char>& data, const size_t n, bool print) {
        if (print) {
            std::cout << "SKIP: ";
            printNext(data, n);
        }
        pos += n;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Printing
    ///////////////////////////////////////////////////////////////////////////

    void ScenarioReader::printOffset() const {
        // Switch to hex, print the value, and switch back
        std::cout
            << "Offset: 0x"
            << std::setw(4)
            << std::setfill('0')
            << std::hex
            << pos
            << '\n'
            << std::dec;
    }

    void ScenarioReader::printSection(std::string title) const {
        std::cout
                << "\n==================================================\n"
                << title << '\n'
                << "==================================================\n\n";
    }

    void ScenarioReader::printNext(
            std::vector<unsigned char>& data, const size_t n) const {

        // Switch to hex
        std::cout << std::setfill('0') << std::hex;
        for (size_t i = 0; i < n; i++) {
            unsigned int value = static_cast<unsigned int>(data[pos + i]);
            std::cout << std::setw(2) << value << " ";
        }
        // Switch back to decimal
        std::cout << '\n' << std::dec;
    }

    void ScenarioReader::print(ScenarioHeader& hdr) const {
        std::cout
            << "Unknown1: " << hdr.unknown1 << '\n'
            << "Unknown2: " << hdr.unknown2 << '\n'
            << "Map Name: " << hdr.mapName << '\n'
            << "Map Size: " << hdr.mapWidth << "x" << hdr.mapHeight << '\n';
    }

    void ScenarioReader::print(PlayerProperties& props) const {
        std::cout
            << "Starting Gold:  " << props.startingGold << '\n'
            << "Starting Wood:  " << props.startingWood << '\n'
            << "Starting Food:  " << props.startingFood << '\n'
            << "Race:           " << static_cast<int>(props.race) << '\n'
            << "AI:             " << props.ai << '\n'
            << "AI Type:        " << static_cast<int>(props.aiType) << '\n'
            << "AI Performance: " << static_cast<int>(props.aiPerformance) << '\n'
            << "AI Strategy:    " << static_cast<int>(props.aiStrategy) << '\n';
    }

    void ScenarioReader::print(TroopDefaults& troop) const {
        std::cout
            << "Hitpoints: " << troop.hitpoints << '\n'
            << "Magic:     " << troop.magic << '\n'
            << "Armour:    " << static_cast<int>(troop.armour) << '\n'
            << "Sight:     " << static_cast<int>(troop.sight) << '\n'
            << "Range:     " << static_cast<int>(troop.range) << '\n';
    }

    void ScenarioReader::print(UpgradeProperties& upgrade) const {
        std::cout
            << "Amount:    " << upgrade.amount << '\n'
            << "Gold Cost: " << upgrade.goldCost << '\n'
            << "Wood Cost: " << upgrade.woodCost << '\n'
            << "Unknown:   " << upgrade.unknown << '\n';
    }

    void ScenarioReader::print(ProductionCost& cost) const {
        std::cout
            << "Gold Cost:         " << cost.goldCost << '\n'
            << "Wood Cost:         " << cost.woodCost << '\n'
            << "Construction Time: " << cost.constructionTime << '\n'
            << "XP or Increase:    " << cost.requiredExpOrIncreasePercent << '\n';
    }

    void ScenarioReader::print(WeaponDefaults& wpn) const {
        std::cout
            << "Move Spaces:  " << wpn.moveSpaces << '\n'
            << "Move Time:    " << wpn.moveSpaces << '\n'
            << "Damage:       " << wpn.damage << '\n'
            << "Penetrate:    " << wpn.penetrate << '\n'
            << "Accuracy:     " << wpn.accuracy << '\n'
            << "Effect Range: " << static_cast<int>(wpn.effectRange) << '\n'
            << "Attack Range: " << static_cast<int>(wpn.attackRange) << '\n'
            << "Mana Cost:    " << wpn.manaCost << '\n'
            << "Reload Time:  " << wpn.reloadTime << '\n'
            << "Unknown:      " << wpn.unknown << '\n';
    }

    void ScenarioReader::print(AvailableBuildings& bldg) const {
        std::cout
            << "Crop Land:                  " << static_cast<int>(bldg.cropLand) << '\n'
            << "Gold Amplifier:             " << static_cast<int>(bldg.goldAmplifier) << '\n'
            << "Ranged Troop Building:      " << static_cast<int>(bldg.rangedTroopBuilding) << '\n'
            << "Siege Troop Building:       " << static_cast<int>(bldg.siegeTroopBuilding) << '\n'
            << "Melee Troop Building:       " << static_cast<int>(bldg.meleeTroopBuilding) << '\n'
            << "Flying Troop Building:      " << static_cast<int>(bldg.flyingTroopBuilding) << '\n'
            << "Engineer Troop Building:    " << static_cast<int>(bldg.engineerTroopBuilding) << '\n'
            << "Healer Troop Building:      " << static_cast<int>(bldg.healerTroopBuilding) << '\n'
            << "Spellcaster Troop Building: " << static_cast<int>(bldg.spellcasterTroopBuilding) << '\n'
            << "Shipyard:                   " << static_cast<int>(bldg.shipyard) << '\n'
            << "Watch Tower:                " << static_cast<int>(bldg.watchTower)  << '\n'
            << "Wall:                       " << static_cast<int>(bldg.wall) << '\n';
    }

    void ScenarioReader::print(HireTroopsRestrictions& restrictions) const {
        std::cout
            << "Worker:                    " << static_cast<int>(restrictions.worker) << '\n'
            << "Ranged Troop:              " << static_cast<int>(restrictions.rangedTroop) << '\n'
            << "Light Melee / Spellcaster: " << static_cast<int>(restrictions.lightMeleeOrSpellcasterTroop) << '\n'
            << "Heavy Melee Troop:         " << static_cast<int>(restrictions.heavyMeleeTroop) << '\n'
            << "Engineer:                  " << static_cast<int>(restrictions.engineer) << '\n'
            << "Stealth Troop:             " << static_cast<int>(restrictions.stealthTroop) << '\n'
            << "Siege Troop:               " << static_cast<int>(restrictions.siegeTroop) << '\n'
            << "Race Bonus Troop:          " << static_cast<int>(restrictions.raceBonusTroop) << '\n'
            << "Spellcaster:               " << static_cast<int>(restrictions.spellcaster) << '\n'
            << "Healer:                    " << static_cast<int>(restrictions.healer) << '\n'
            << "Transport Ship:            " << static_cast<int>(restrictions.transportShip) << '\n'
            << "Combat Ship:               " << static_cast<int>(restrictions.combatShip) << '\n'
            << "Flying Troop:              " << static_cast<int>(restrictions.flyingTroop) << '\n'
            << "Flying Transport:          " << static_cast<int>(restrictions.flyingTransport) << '\n'
            << "Must Hire:                 " << static_cast<int>(restrictions.mustHire) << '\n';
    }

    void ScenarioReader::print(BuildingPlacement& bldg) const {
        std::cout
            << "Type:           " << static_cast<int>(bldg.type) << '\n'
            << "Player:         " << static_cast<int>(bldg.player) << '\n'
            << "X:              " << bldg.x << '\n'
            << "Y:              " << bldg.y << '\n'
            << "Hitpoints:      " << bldg.hitpoints << '\n'
            << "Armour:         " << bldg.armour << '\n'
            << "Sight:          " << static_cast<int>(bldg.sight) << '\n'
            << "Range:          " << static_cast<int>(bldg.range) << '\n'
            << "Upgrade 1:      " << bldg.upgrade1Enabled << '\n'
            << "Upgrade 2:      " << bldg.upgrade2Enabled << '\n'
            << "Special Colour: " << static_cast<int>(bldg.specialColour) << '\n'
            << "Prisoner:       " << bldg.prisoner << '\n'
            << "Name:           " << bldg.name << '\n';
    }

    void ScenarioReader::print(UnitPlacement& unit) const {
        std::cout
            << "Type:           " << static_cast<int>(unit.type) << '\n'
            << "Facing:         " << static_cast<int>(unit.facing) << '\n'
            << "X:              " << unit.x << '\n'
            << "Y:              " << unit.y << '\n'
            << "Player:         " << static_cast<int>(unit.player) << '\n'
            << "Hitpoints:      " << unit.hitpoints << '\n'
            << "Magic:          " << static_cast<int>(unit.magic) << '\n'
            << "Armour:         " << unit.armour << '\n'
            << "Type2:          " << static_cast<int>(unit.type2) << '\n'
            << "Sight:          " << static_cast<int>(unit.sight) << '\n'
            << "Range:          " << static_cast<int>(unit.range) << '\n'
            << "Special Colour: " << static_cast<int>(unit.specialColour) << '\n'
            << "Prisoner:       " << unit.prisoner << '\n'
            << "Gold Cost:      " << unit.goldCost << '\n'
            << "Wood Cost:      " << unit.woodCost << '\n'
            << "Name:           " << unit.name << '\n'
            << "Upgrade 1:      " << unit.upgrade1Enabled << '\n'
            << "Upgrade 2:      " << unit.upgrade2Enabled << '\n'
            << "Upgrade 3:      " << unit.upgrade3Enabled << '\n'
            << "Upgrade 4:      " << unit.upgrade4Enabled << '\n'
            << "Fighting Area:  " << static_cast<int>(unit.fightingArea) << '\n';
    }

    void ScenarioReader::print(TrapPlacement& trap) const {
        std::cout
            << "X:              " << static_cast<int>(trap.x) << '\n'
            << "Y:              " << static_cast<int>(trap.y) << '\n'
            << "Player:         " << static_cast<int>(trap.player) << '\n';
    }

}
