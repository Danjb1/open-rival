#include "pch.h"
#include "ScenarioReader.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

namespace Rival {

    const std::map<std::uint8_t, char> ScenarioReader::alphabet = {
        { 0x12, 'b' },
        { 0x13, 'c' },
        { 0x15, 'a' },
        { 0x16, 'f' },
        { 0x17, 'g' },
        { 0x18, 'd' },
        { 0x19, 'e' },
        { 0x1A, 'j' },
        { 0x1B, 'k' },
        { 0x1C, 'h' },
        { 0x1D, 'i' },
        { 0x1E, 'n' },
        { 0x1F, 'o' },
        { 0x20, 'l' },
        { 0x21, 'm' },
        { 0x22, 'r' },
        { 0x23, 's' },
        { 0x24, 'p' },
        { 0x26, 'v' },
        { 0x27, 'w' },
        { 0x28, 't' },
        { 0x29, 'u' },
        { 0x2A, 'z' },
        { 0x2C, 'x' },
        { 0x2D, 'y' },
        { 0x32, 'B' },
        { 0x33, 'C' },
        { 0x35, 'A' },
        { 0x36, 'F' },
        { 0x37, 'G' },
        { 0x38, 'D' },
        { 0x39, 'E' },
        { 0x3A, 'J' },
        { 0x3B, 'K' },
        { 0x3C, 'H' },
        { 0x3D, 'I' },
        { 0x3E, 'N' },
        { 0x3F, 'O' },
        { 0x40, 'L' },
        { 0x41, 'M' },
        { 0x42, 'R' },
        { 0x43, 'S' },
        { 0x44, 'P' },
        { 0x46, 'V' },
        { 0x47, 'W' },
        { 0x48, 'T' },
        { 0x49, 'U' },
        { 0x4A, 'Z' },
        { 0x4C, 'X' },
        { 0x4D, 'Y' },
        { 0x52, '"' },
        { 0x54, ' ' },
        { 0x56, '&' },
        { 0x60, ',' },
        { 0x63, '3' },
        { 0x62, '2' },
        { 0x81, '\r' },
        { 0x7A, '\n' }
    };

    ///////////////////////////////////////////////////////////////////////////
    // Entry points
    ///////////////////////////////////////////////////////////////////////////

    ScenarioReader::ScenarioReader(const std::string filename) {

        try {
            readFileContents(filename);
        } catch (const std::exception& e) {
            std::cerr << e.what();
            throw std::runtime_error("Failed to read scenario: " + filename);
        }
    }

    void ScenarioReader::readFileContents(const std::string filename) {

        // Open the file at the end
        std::ifstream is(filename, std::ios::binary | std::ios::ate);
        if (!is.is_open()) {
            throw std::runtime_error("Failed to open scenario: " + filename);
        }

        // Create a buffer to hold the entire file contents
        std::streampos size = is.tellg();
        if (size == -1) {
            throw std::runtime_error("Failed to retrieve file size");
        }
        data = std::vector<unsigned char>(static_cast<size_t>(size));

        // Read the entire file to memory
        is.seekg(0, std::ios::beg);
        is.read(reinterpret_cast<char*>(data.data()), size);
        is.close();
    }

    ScenarioFile ScenarioReader::readScenario() {
        pos = 0;
        ScenarioFile scenarioFile = parseScenario(true);
        return scenarioFile;
    }

    ScenarioFile ScenarioReader::readCampaignScenario(int levelIndex) {
        pos = 0;
        std::uint8_t numLevels = readByte();

        // Skip entries for all scenarios before this one
        for (int i = 0; i < levelIndex; i++) {
            skip(8, false);
        }

        // Read this scenario's offset and size
        std::uint32_t offset = readInt();
        std::uint32_t size = readInt();

        // Parse the scenario
        pos = offset;
        ScenarioFile scenarioFile = parseScenario(false);
        return scenarioFile;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Parsing
    ///////////////////////////////////////////////////////////////////////////

    ScenarioFile ScenarioReader::parseScenario(bool expectEof) {

        ScenarioFile scenarioFile;

        // Parse map header
        printSection("Parsing header");
        printOffset();
        scenarioFile.hdr = parseHeader();
        print(scenarioFile.hdr);

        // Parse player properties
        printSection("Parsing player properties");
        printOffset();
        for (int i = 0; i < numPlayers; i++) {
            scenarioFile.playerProperties[i] = parsePlayerProperties();
        }
        print(scenarioFile.playerProperties[0]);

        // UNKNOWN
        printSection("Skipping unknown section");
        printOffset();
        skip(909, true);

        /*
        24 -- -- -- -- b0 04 0c -- -- 08 -- -- -- ff --
        -- -- -- -- -- -- -- -- -- -- -- -- -- -- 58 02
        04 -- -- 05 -- -- -- ff -- -- -- -- -- -- -- --
        -- -- -- -- -- -- -- 84 03 08 -- -- 06 -- -- --
        ff -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        84 03 08 -- -- 06 -- -- -- ff -- -- -- -- -- --
        -- -- -- -- -- -- -- -- -- 84 03 08 -- -- 06 --
        -- -- ff -- -- -- -- -- -- -- -- -- -- -- -- --
        -- -- 84 03 08 -- -- 06 -- -- -- ff -- -- -- --
        -- -- -- -- -- -- -- -- -- -- -- 84 03 08 -- --
        06 -- -- -- ff -- -- -- -- -- -- -- -- -- -- --
        -- -- -- -- 84 03 08 -- -- 06 -- -- -- ff -- --
        -- -- -- -- -- -- -- -- -- -- -- -- -- 84 03 08
        -- -- 06 -- -- -- ff -- -- -- -- -- -- -- -- --
        -- -- -- -- -- -- 84 03 08 -- -- 06 -- -- -- ff
        -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 84
        03 0c -- -- 08 -- -- -- ff -- -- -- -- -- -- --
        -- -- -- -- -- -- -- -- 58 02 -- -- -- 04 -- --
        -- ff -- -- -- -- -- -- -- -- -- -- -- -- -- --
        -- b0 04 0c -- -- 08 -- -- -- ff -- -- -- -- --
        -- -- -- -- -- -- -- -- -- -- 58 02 04 -- -- 05
        -- -- -- ff -- -- -- -- -- -- -- -- -- -- -- --
        -- -- -- 84 03 08 -- -- 06 -- -- -- ff -- -- --
        -- -- -- -- -- -- -- -- -- -- -- -- 84 03 08 --
        -- 06 -- -- -- ff -- -- -- -- -- -- -- -- -- --
        -- -- -- -- -- 84 03 08 -- -- 06 -- -- -- ff --
        -- -- -- -- -- -- -- -- -- -- -- -- -- -- 84 03
        08 -- -- 06 -- -- -- ff -- -- -- -- -- -- -- --
        -- -- -- -- -- -- -- 84 03 08 -- -- 06 -- -- --
        ff -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        84 03 08 -- -- 06 -- -- -- ff -- -- -- -- -- --
        -- -- -- -- -- -- -- -- -- 84 03 08 -- -- 06 --
        -- -- ff -- -- -- -- -- -- -- -- -- -- -- -- --
        -- -- 84 03 08 -- -- 06 -- -- -- ff -- -- -- --
        -- -- -- -- -- -- -- -- -- -- -- 84 03 0c -- --
        08 -- -- -- ff -- -- -- -- -- -- -- -- -- -- --
        -- -- -- -- 58 02 -- -- -- 04 -- -- -- ff -- --
        -- -- -- -- -- -- -- -- -- -- -- -- -- b0 04 0c
        -- -- 08 -- -- -- ff -- -- -- -- -- -- -- -- --
        -- -- -- -- -- -- 58 02 04 -- -- 05 -- -- -- ff
        -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 84
        03 08 -- -- 06 -- -- -- ff -- -- -- -- -- -- --
        -- -- -- -- -- -- -- -- 84 03 08 -- -- 06 -- --
        -- ff -- -- -- -- -- -- -- -- -- -- -- -- -- --
        -- 84 03 08 -- -- 06 -- -- -- ff -- -- -- -- --
        -- -- -- -- -- -- -- -- -- -- 84 03 08 -- -- 06
        -- -- -- ff -- -- -- -- -- -- -- -- -- -- -- --
        -- -- -- 84 03 08 -- -- 06 -- -- -- ff -- -- --
        -- -- -- -- -- -- -- -- -- -- -- -- 84 03 08 --
        -- 06 -- -- -- ff -- -- -- -- -- -- -- -- -- --
        -- -- -- -- -- 84 03 08 -- -- 06 -- -- -- ff --
        -- -- -- -- -- -- -- -- -- -- -- -- -- -- 84 03
        08 -- -- 06 -- -- -- ff -- -- -- -- -- -- -- --
        -- -- -- -- -- -- -- 84 03 0c -- -- 08 -- -- --
        ff -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        58 02 -- -- -- 04 -- -- -- ff -- -- -- -- -- --
        -- -- -- -- -- -- -- -- 2a -- -- -- --
        */

        // Parse troop defaults
        printSection("Parsing troop defaults");
        printOffset();
        for (int i = 0; i < numTroops; i++) {
            scenarioFile.troopDefaults[i] = parseTroopDefaults();
        }
        print(scenarioFile.troopDefaults[0]);
        skip(7, true);

        // Parse upgrade properties
        printSection("Parsing upgrade properties");
        printOffset();
        for (int i = 0; i < numUpgrades; i++) {
            scenarioFile.upgradeProperties[i] =
                    parseUpgradeProperties(doesUpgradeHaveAmount(i));
        }
        print(scenarioFile.upgradeProperties[0]);

        // UNKNOWN
        printSection("Skipping unknown section");
        printOffset();
        skip(176, true);

        /*
        ff ff ff ff d2 -- -- -- -- -- -- -- 34 -- -- --
        01 19 ff 02 64 ff 03 fa ff 04 0a ff 05 64 ff 06
        03 ff 07 03 ff 08 32 ff 09 64 ff 0a c8 ff 0b 32
        ff 0c 64 ff 0d -- 01 0e 03 ff 0f 06 ff 10 09 ff
        11 0f ff 12 04 ff 13 06 ff 14 08 ff 15 0a ff 16
        0f ff 17 14 ff 18 01 ff 19 01 ff 1a 01 ff 1b 01
        ff 1c 01 ff 1d 0f ff 1e 14 ff 1f 1e ff 20 64 ff
        21 c8 ff 22 01 ff 23 03 3c 24 03 5e 25 03 5f 26
        03 5d 27 03 10 28 03 22 29 03 34 2a 03 60 2b 03
        25 2c 03 35 2d 03 27 2e 03 15 2f 03 38 30 03 36
        31 01 5c 32 01 5c 33 01 5c 34 01 5c 4f -- -- --
        */

        // Parse unit production costs
        printSection("Parsing unit production costs");
        printOffset();
        for (int i = 0; i < numProductionCosts; i++) {
            scenarioFile.productionCosts[i] = parseProductionCost();
        }
        print(scenarioFile.productionCosts[0]);
        skip(27, true);

        // Parse weapon defaults
        printSection("Parsing weapon defaults");
        printOffset();
        for (int i = 0; i < numWeapons; i++) {
            scenarioFile.weaponDefaults[i] = parseWeaponDefaults();
        }
        print(scenarioFile.weaponDefaults[0]);
        skip(2, true);

        // Parse available buildings
        printSection("Parsing available buildings");
        printOffset();
        scenarioFile.availableBuildings = parseAvailableBuildings();
        print(scenarioFile.availableBuildings);
        skip(5, true);

        // Parse monster defaults
        printSection("Parsing monster defaults");
        printOffset();
        for (int i = 0; i < numMonsters; i++) {
            scenarioFile.monsterDefaults[i] = parseTroopDefaults();
        }
        print(scenarioFile.monsterDefaults[0]);
        skip(49, true);

        // Parse hire troops restrictions
        printSection("Parsing hire troops restrictions");
        printOffset();
        scenarioFile.hireTroopsRestrictions =
                parseHireTroopsRestrictions();
        print(scenarioFile.hireTroopsRestrictions);

        // Parse AI building settings
        printSection("Parsing AI building settings");
        printOffset();
        for (int i = 0; i < numBuildingsPerRace; i++) {
            for (int j = 0; j < numAiStrategies; j++) {
                scenarioFile.aiStrategies[j].aiBuildingSettings[i] =
                        parseAiSetting();
            }
        }

        // Parse AI troop settings
        printSection("Parsing AI troop settings");
        printOffset();
        for (int i = 0; i < numTroopsPerRace; i++) {
            for (int j = 0; j < numAiStrategies; j++) {
                scenarioFile.aiStrategies[j].aiTroopSettings[i] =
                        parseAiSetting();
            }
        }

        // UNKNOWN
        printSection("Skipping unknown section");
        printOffset();
        skip(392, true);

        /*
        0a -- 0a -- 0a -- 0a -- 0a -- 0a -- 0a -- 04 --
        04 -- 01 -- 02 -- 02 -- 02 -- 02 -- 02 -- 02 --
        -- -- -- -- -- -- -- -- -- -- 04 -- 03 -- 01 --
        01 -- 01 -- 01 -- 01 -- 02 03 02 03 02 02 02 03
        03 02 03 02 03 02 -- -- 01 01 -- -- -- -- -- --
        -- -- -- -- 01 -- 01 -- 02 -- -- -- -- -- -- --
        -- -- -- -- 01 -- -- -- -- -- -- -- -- -- -- --
        01 -- 02 -- 01 -- 03 -- 02 -- 02 -- 02 -- -- --
        01 -- -- -- 01 -- -- -- -- -- -- -- -- -- -- --
        -- -- -- -- -- -- -- -- -- -- -- -- -- -- 04 --
        -- -- -- -- -- -- -- -- -- -- 03 -- 03 -- 02 --
        04 -- 04 -- 04 -- -- -- 01 02 01 02 01 02 01 02
        01 02 01 02 0a -- 0a -- 0a -- 0a -- 0a -- 0a --
        0a -- 04 -- 04 -- 01 -- 02 -- 02 -- 02 -- 02 --
        02 -- 02 -- -- -- -- -- -- -- -- -- -- -- 04 --
        03 -- 01 -- 01 -- 01 -- 01 -- 01 -- 02 03 02 03
        02 02 02 03 03 02 03 02 03 02 -- -- 01 01 -- --
        -- -- -- -- -- -- -- -- 01 -- 01 -- 02 -- -- --
        -- -- -- -- -- -- -- -- 01 -- -- -- -- -- -- --
        -- -- -- -- 01 -- 02 -- 01 -- 03 -- 02 -- 02 --
        02 -- -- -- 01 -- -- -- 01 -- -- -- -- -- -- --
        -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        -- -- 04 -- -- -- -- -- -- -- -- -- -- -- 03 --
        03 -- 02 -- 04 -- 04 -- 04 -- -- -- 01 02 01 02
        01 02 01 02 01 02 01 02
        */

        // Parse terrain data
        printSection("Parsing terrain data");
        printOffset();
        scenarioFile.tiles = parseTerrain(
                scenarioFile.hdr.mapWidth, scenarioFile.hdr.mapHeight);

        // Parse objects
        printSection("Parsing objects");
        printOffset();
        scenarioFile.objects = parseObjects();
        std::cout << "Found "
            << scenarioFile.objects.size() << " object(s)\n";
        if (scenarioFile.objects.size() > 0) {
            print(scenarioFile.objects[0]);
        }

        // Parse buildings
        printSection("Parsing buildings");
        printOffset();
        scenarioFile.buildings = parseBuildings();
        std::cout << "Found "
                << scenarioFile.buildings.size() << " building(s)\n";
        if (scenarioFile.buildings.size() > 0) {
            print(scenarioFile.buildings[0]);
        }

        // Parse units
        printSection("Parsing units");
        printOffset();
        scenarioFile.units = parseUnits();
        std::cout << "Found "
                << scenarioFile.units.size() << " unit(s)\n";
        if (scenarioFile.units.size() > 0) {
            print(scenarioFile.units[0]);
        }

        // Parse chests
        printSection("Parsing chests");
        printOffset();
        scenarioFile.chests = parseChests();
        std::cout << "Found "
            << scenarioFile.chests.size() << " chest(s)\n";
        if (scenarioFile.chests.size() > 0) {
            //print(scenarioFile.chests[0]);
        }

        // Parse info points
        printSection("Parsing info points");
        printOffset();
        scenarioFile.infoPoints = parseInfoPoints();
        std::cout << "Found "
            << scenarioFile.infoPoints.size() << " info point(s)\n";
        if (scenarioFile.infoPoints.size() > 0) {
            //print(scenarioFile.infoPoints[0]);
        }

        // Parse traps
        printSection("Parsing traps");
        printOffset();
        scenarioFile.traps = parseTraps();
        std::cout << "Found "
                << scenarioFile.traps.size() << " trap(s)\n";
        if (scenarioFile.traps.size() > 0) {
            print(scenarioFile.traps[0]);
        }
        skip(8, true);

        // Parse goal locations
        printSection("Parsing goal locations");
        printOffset();
        scenarioFile.goalLocations = parseGoalLocations();
        std::cout << "Found "
            << scenarioFile.goalLocations.size() << " goal location(s)\n";
        if (scenarioFile.goalLocations.size() > 0) {
            print(scenarioFile.goalLocations[0]);
        }

        // Parse scenario goals
        printSection("Parsing scenario goals");
        printOffset();
        scenarioFile.goals = parseGoals();

        // Parse campaign texts
        printSection("Parsing campaign texts");
        printOffset();
        scenarioFile.campaignText = parseCampaignText();
        print(scenarioFile.campaignText);
        printOffset();

        // Parse checksum
        printSection("Parsing checksum");
        printOffset();
        scenarioFile.checksum = readByte();

        // Check remaining bytes
        if (expectEof) {
            size_t remainingBytes = data.size() - pos;
            if (remainingBytes == 0) {
                std::cout << "Reached end of file\n";
            } else {
                std::cout << "Found unexpected bytes:\n";
                size_t remainingBytesCapped =
                    std::min(remainingBytes, static_cast<size_t>(256));
                printNext(remainingBytesCapped);
                throw std::runtime_error("Did not reach end of file");
            }
        }

        return scenarioFile;
    }

    ScenarioHeader ScenarioReader::parseHeader() {

        ScenarioHeader hdr;

        skip(8, true);
        std::uint8_t terrainType = readByte();
        hdr.wilderness = (terrainType == 0x03);
        std::uint8_t nameLength = readByte();
        hdr.mapName = readString(nameLength);
        hdr.mapHeight = readInt();
        hdr.mapWidth = readInt();
        skip(9, false);

        return hdr;
    }

    PlayerProperties ScenarioReader::parsePlayerProperties() {

        PlayerProperties props;

        props.hasStartLocation = (readInt() == 1);
        props.startLocX = readInt();
        props.startLocY = readInt();
        props.startingFood = readInt();
        props.startingWood = readInt();
        props.startingGold = readInt();
        props.race = readByte();
        props.ai = readBool();
        props.aiType = readByte();
        props.aiPerformance = readByte();
        props.aiStrategy = readByte();

        return props;
    }

    TroopDefaults ScenarioReader::parseTroopDefaults() {

        TroopDefaults troop;

        troop.hitpoints = readShort();
        troop.magic = readShort();
        troop.armour = readByte();
        skip(2, false);
        troop.sight = readByte();
        troop.range = readByte();
        skip(39, false);

        return troop;
    }

    bool ScenarioReader::doesUpgradeHaveAmount(int i) const {
        // Upgrades 0 - 47 and 67 - 69 do not have an "Amount" field
        return i <= 47 || (i >= 67 && i <= 69);
    }

    UpgradeProperties ScenarioReader::parseUpgradeProperties(
            bool readAmount) {

        UpgradeProperties upgrade;

        if (readAmount) {
            upgrade.amount = readInt();
        } else {
            upgrade.amount = 0;
            skip(4, false);
        }
        upgrade.goldCost = readInt();
        upgrade.woodCost = readInt();
        upgrade.unknown = readInt();

        return upgrade;
    }

    ProductionCost ScenarioReader::parseProductionCost() {

        ProductionCost cost;

        cost.goldCost = readShort();
        cost.woodCost = readShort();
        cost.constructionTime = readInt();
        cost.requiredExpOrIncreasePercent = readInt();

        return cost;
    }

    WeaponDefaults ScenarioReader::parseWeaponDefaults() {

        WeaponDefaults weapon;

        weapon.moveSpaces = readShort();
        weapon.moveTime = readShort();
        weapon.damage = readShort();
        weapon.penetrate = readShort();
        weapon.accuracy = readShort();
        weapon.effectRange = readByte();
        weapon.attackRange = readByte();
        weapon.manaCost = readShort();
        weapon.reloadTime = readInt();
        weapon.unknown = readShort();
        skip(1, false);

        return weapon;
    }

    AvailableBuildings ScenarioReader::parseAvailableBuildings() {

        AvailableBuildings bldgs;

        bldgs.cropLand = readBool();
        bldgs.goldAmplifier = readBool();
        bldgs.rangedTroopBuilding = readBool();
        bldgs.siegeTroopBuilding = readBool();
        bldgs.meleeTroopBuilding = readBool();
        bldgs.flyingTroopBuilding = readBool();
        bldgs.engineerTroopBuilding = readBool();
        bldgs.healerTroopBuilding = readBool();
        bldgs.spellcasterTroopBuilding = readBool();
        bldgs.shipyard = readBool();
        bldgs.watchTower = readBool();
        bldgs.wall = readBool();

        return bldgs;
    }

    HireTroopsRestrictions ScenarioReader::parseHireTroopsRestrictions() {

        HireTroopsRestrictions restrictions;

        restrictions.worker = readBool();
        restrictions.rangedTroop = readBool();
        restrictions.lightMeleeOrSpellcasterTroop = readBool();
        restrictions.heavyMeleeTroop = readBool();
        restrictions.engineer = readBool();
        restrictions.stealthTroop = readBool();
        restrictions.siegeTroop = readBool();
        restrictions.raceBonusTroop = readBool();
        restrictions.spellcaster = readBool();
        restrictions.healer = readBool();
        restrictions.transportShip = readBool();
        restrictions.combatShip = readBool();
        restrictions.flyingTroop = readBool();
        restrictions.flyingTransport = readBool();
        restrictions.mustHire = readBool();

        return restrictions;
    }

    AiSetting ScenarioReader::parseAiSetting() {
        AiSetting setting;
        setting.amount = readByte();
        setting.flag = readByte();
        return setting;
    }

    std::vector<TilePlacement> ScenarioReader::parseTerrain(
            int width, int height) {

        unsigned int numTiles = width * height;
        std::vector<TilePlacement> tiles(numTiles);
        size_t tileId = 0;

        while (tileId < numTiles) {

            // Determine how many tiles the next tile definition describes,
            // and skip to the tile definition
            bool readMultiple = (readByte() == 0xfa);
            std::uint16_t numTiles = 1;
            if (readMultiple) {
                numTiles = readShort();
                skip(4, false);
            } else {
                skip(3, false);
            }

            // Read the tile definition
            TilePlacement tile = parseTile();

            // Copy the tile definition to the number of tiles it describes
            for (size_t i = 0; i < numTiles; i++) {
                tiles.push_back(tile);
                tileId++;
            }

            pos += bytesPerTile;
        }

        return tiles;
    }

    TilePlacement ScenarioReader::parseTile() {

        TilePlacement tile;
        std::uint8_t resource = readByte(pos);

        if (resource == 0) {

            std::uint8_t type = readByte(pos + 2);

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
                printNext(6);
            }

        } else if (resource == 1) {
            // Gold
            tile.type = TileType::Gold;

        } else if (resource == 2) {
            // Cropland
            tile.type = TileType::Cropland;

        } else {
            std::cout << "Unknown tile: ";
            printNext(6);
        }

        return tile;
    }

    std::vector<ObjectPlacement> ScenarioReader::parseObjects() {

        std::uint32_t numObjects = readInt();
        std::vector<ObjectPlacement> objects;
        objects.reserve(numObjects);

        for (unsigned int i = 0; i < numObjects; i++) {
            ObjectPlacement obj = parseObject();
            objects.push_back(obj);
        }

        return objects;
    }

    ObjectPlacement ScenarioReader::parseObject() {

        ObjectPlacement obj;

        obj.type = readByte();
        skip(1, false);
        obj.variant = readByte();
        obj.x = readInt();
        obj.y = readInt();

        return obj;
    }

    std::vector<BuildingPlacement> ScenarioReader::parseBuildings() {

        std::uint32_t numBuildings = readInt();
        std::vector<BuildingPlacement> buildings;
        buildings.reserve(numBuildings);

        for (unsigned int i = 0; i < numBuildings; i++) {
            BuildingPlacement bldg = parseBuilding();
            buildings.push_back(bldg);
        }

        return buildings;
    }

    BuildingPlacement ScenarioReader::parseBuilding() {

        BuildingPlacement bldg;

        bldg.type = readByte();
        bldg.player = readByte();
        skip(1, true);
        skip(1, true);
        skip(1, true);
        bldg.x = readShort();
        bldg.y = readShort();
        skip(1, true);
        bldg.hitpoints = readShort();
        bldg.armour = readShort();
        skip(1, false);
        bldg.sight = readByte();
        bldg.range = readByte();
        bldg.upgrade1Enabled = readBool();
        bldg.upgrade2Enabled = readBool();
        bldg.specialColour = readByte();
        bldg.prisoner = readBool();
        bldg.name = readString(12);
        skip(1, false);

        return bldg;
    }

    std::vector<UnitPlacement> ScenarioReader::parseUnits() {

        std::uint32_t numUnits = readInt();
        std::vector<UnitPlacement> units;
        units.reserve(numUnits);

        for (unsigned int i = 0; i < numUnits; i++) {
            UnitPlacement unit = parseUnit();
            units.push_back(unit);
        }

        return units;
    }

    UnitPlacement ScenarioReader::parseUnit() {

        UnitPlacement unit;

        unit.type = readByte();
        skip(2, true);
        unit.facing = readByte();
        skip(1, true);
        unit.x = readShort();
        unit.y = readShort();
        unit.player = readByte();
        unit.hitpoints = readShort();
        unit.magic = readByte();
        unit.armour = readShort();
        skip(1, true);
        unit.type2 = readByte();
        unit.sight = readByte();
        unit.range = readByte();
        skip(2, true);
        unit.specialColour = readByte();
        unit.prisoner = readBool();
        unit.goldCost = readShort();
        unit.woodCost = readShort();
        unit.name = readString(12);
        skip(13, true);
        unit.upgrade1Enabled = readBool();
        unit.upgrade2Enabled = readBool();
        unit.upgrade3Enabled = readBool();
        unit.upgrade4Enabled = readBool();
        unit.fightingArea = readByte();

        return unit;
    }

    std::vector<ChestPlacement> ScenarioReader::parseChests() {

        std::uint32_t numChests = readInt();
        std::vector<ChestPlacement> chests;
        chests.reserve(numChests);

        for (unsigned int i = 0; i < numChests; i++) {
            ChestPlacement chest = parseChest();
            chests.push_back(chest);
        }

        return chests;
    }

    ChestPlacement ScenarioReader::parseChest() {

        ChestPlacement obj;

        obj.type = readInt();
        obj.variant = readInt();
        obj.x = readByte();
        obj.y = readByte();
        skip(10, true); // items

        return obj;
    }

    std::vector<InfoPointPlacement> ScenarioReader::parseInfoPoints() {

        std::uint32_t numInfoPoints = readInt();
        std::vector<InfoPointPlacement> infoPoints;
        infoPoints.reserve(numInfoPoints);

        for (unsigned int i = 0; i < numInfoPoints; i++) {
            InfoPointPlacement infoPoint = parseInfoPoint();
            infoPoints.push_back(infoPoint);
        }

        return infoPoints;
    }

    InfoPointPlacement ScenarioReader::parseInfoPoint() {

        InfoPointPlacement infoPoint;

        infoPoint.x = readByte();
        infoPoint.y = readByte();
        std::uint16_t messageLength = readShort();
        infoPoint.message = readString(messageLength);

        return infoPoint;
    }

    std::vector<TrapPlacement> ScenarioReader::parseTraps() {

        std::uint32_t numTraps = readInt();
        std::vector<TrapPlacement> traps;
        traps.reserve(numTraps);

        for (unsigned int i = 0; i < numTraps; i++) {
            TrapPlacement trap = parseTrap();
            traps.push_back(trap);
        }

        return traps;
    }

    TrapPlacement ScenarioReader::parseTrap() {

        TrapPlacement trap;

        trap.x = readByte();
        trap.y = readByte();
        trap.player = readByte();

        return trap;
    }

    std::vector<GoalLocation> ScenarioReader::parseGoalLocations() {

        std::uint8_t numGoalLocations = readRivalByte();
        std::vector<GoalLocation> goals;
        goals.reserve(numGoalLocations);

        for (unsigned int i = 0; i < numGoalLocations; i++) {
            GoalLocation goal = parseGoalLocation();
            goals.push_back(goal);
        }

        return goals;
    }

    GoalLocation ScenarioReader::parseGoalLocation() {

        GoalLocation goal;

        goal.type = readRivalByte();
        goal.x = readRivalByte();
        skip(3, false);
        goal.y = readRivalByte();
        skip(3, false);

        return goal;
    }

    std::vector<Goal> ScenarioReader::parseGoals() {

        std::uint8_t numGoals = readRivalByte();

        if (numGoals > 0) {
            skip(3, false);
        }

        std::vector<Goal> goals;
        goals.reserve(numGoals);

        for (unsigned int i = 0; i < numGoals; i++) {
            Goal goal = parseGoal();
            goals.push_back(goal);
        }

        return goals;
    }

    Goal ScenarioReader::parseGoal() {

        Goal goal;

        std::uint8_t goalType = readByte();
        skip(11, true);

        // TODO: parse goal based on type
        goal.count = 0;

        return goal;
    }

    CampaignText ScenarioReader::parseCampaignText() {

        CampaignText text;

        std::uint16_t titleLength = readRivalShort();
        text.title = readRivalString(titleLength);

        std::uint16_t objectivesLength = readRivalShort();
        text.objectives = readRivalString(objectivesLength);

        std::uint16_t narrationLength = readRivalShort();
        text.narration = readRivalString(narrationLength);

        return text;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Token Extraction
    ///////////////////////////////////////////////////////////////////////////

    std::uint8_t ScenarioReader::readByte() {
        std::uint8_t value = readByte(pos);
        pos += 1;
        return value;
    }

    std::uint8_t ScenarioReader::readByte(size_t offset) const {
        return std::uint8_t(data[offset]);
    }

    std::uint8_t ScenarioReader::readRivalByte() {
        std::uint8_t value = readRivalByte(pos);
        pos += 1;
        return value;
    }

    // Special numbering used by goals, etc.
    std::uint8_t ScenarioReader::readRivalByte(size_t offset) const {
        std::uint8_t val = std::uint8_t(data[offset]);
        return fixRivalByte(val);
    }

    /**
     * Converts "rival bytes" back to their original values.
     *
     * The game stores some numbers in this rather peculiar format.
     *
     * Firstly, some offset is applied to the numbers before they are stored.
     * This offset varies for different "tiers" of numbers.
     *
     * Confusingly, a further offset is also applied to every other pair of numbers.
     */
    std::uint8_t ScenarioReader::fixRivalByte(std::uint8_t val) const {

        // Apply tiered offset
        if (val < 0x12) {
            val += 0x8C;
        } else if (val < 0x32) {
            val += 0x4C;
        } else if (val < 0x52) {
            val -= 0x14;
        } else if (val < 0x72) {
            val -= 0x34;
        } else if (val < 0x92) {
            val -= 0x74;
        } else if (val < 0xB2) {
            val += 0x4C;
        } else if (val < 0xD2) {
            val += 0x0C;
        } else if (val < 0xF2) {
            val -= 0x34;
        } else {
            val -= 0x74;
        }

        // Apply a further offset to every other pair of values
        if ((val & 0x02) > 0) {
            val += 0x04;
        }

        return val;
    }

    std::uint16_t ScenarioReader::readRivalShort() {
        std::uint16_t value = readRivalShort(pos);
        pos += numBytesShort;
        return value;
    }

    uint16_t ScenarioReader::readRivalShort(size_t offset) const {
        // read 2 rival bytes, and combine them like a normal short
        return std::uint16_t(
            fixRivalByte(data[offset + 1]) << 8 |
            fixRivalByte(data[offset + 0])
        );
    }

    bool ScenarioReader::readBool() {
        bool value = readBool(pos);
        pos += 1;
        return value;
    }

    bool ScenarioReader::readBool(size_t offset) const {
        uint8_t value = data[offset];
        return value == 1;
    }

    std::uint16_t ScenarioReader::readShort() {
        std::uint16_t value = readShort(pos);
        pos += numBytesShort;
        return value;
    }

    uint16_t ScenarioReader::readShort(size_t offset) const {
        // little endian
        return std::uint16_t(
            data[offset + 1] << 8 |
            data[offset + 0]
        );
    }

    std::uint32_t ScenarioReader::readInt() {
        std::uint32_t value = readInt(pos);
        pos += numBytesInt;
        return value;
    }

    uint32_t ScenarioReader::readInt(size_t offset) const {
        // little endian
        return std::uint32_t(
            data[offset + 3] << 24 |
            data[offset + 2] << 16 |
            data[offset + 1] << 8 |
            data[offset + 0]
        );
    }

    std::string ScenarioReader::readString(size_t length) {
        std::string value = readString(pos, length);
        pos += length;
        return value;
    }

    std::string ScenarioReader::readString(
            size_t offset, size_t length) const {

        std::vector<char> chars(length);
        for (size_t i = 0; i < length; i++) {
            chars[i] = data[offset + i];
        }
        std::string value(chars.data(), length);
        return value;
    }

    std::string ScenarioReader::readRivalString(size_t length) {
        std::string value = readRivalString(pos, length);
        pos += length;
        return value;
    }

    std::string ScenarioReader::readRivalString(
            size_t offset, size_t length) const {

        std::vector<char> chars(length);
        for (size_t i = 0; i < length; i++) {
            std::uint8_t c = data[offset + i];
            chars[i] = getRivalChar(c);
        }
        std::string value(chars.data(), length);
        return value;
    }

    char ScenarioReader::getRivalChar(std::uint8_t c) const {

        if (c == 0x2B || c == 0x31) {
            // Special value to signify red text
            return 0x02;
        }
        if (c == 0x31) {
            // Special value to signify the end of red text
            return 0x03;
        }

        auto it = alphabet.find(c);

        if (it != alphabet.end()) {
            // Entry found
            return it->second;
        }

        // Unknown character
        return '?';
    }

    void ScenarioReader::skip(const size_t n, bool print) {
        if (print) {
            std::cout << "SKIP: ";
            printNext(n);
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

    void ScenarioReader::printNext(const size_t n) const {

        // Switch to hex
        std::cout << std::setfill('0') << std::hex;
        int col = 0;
        for (size_t i = 0; i < n; i++) {
            unsigned int value = static_cast<unsigned int>(data.at(pos + i));

            if (value == 0) {
                std::cout << "-- ";
            } else {
                std::cout << std::setw(2) << value << " ";
            }

            // Print a new line every 16 bytes
            col++;
            if (col == 16) {
                std::cout << '\n';
                col = 0;
            }
        }
        // Switch back to decimal
        std::cout << '\n' << std::dec;
    }

    void ScenarioReader::print(ScenarioHeader& hdr) const {
        std::cout
            << "Map Name: " << hdr.mapName << '\n'
            << "Map Size: " << hdr.mapWidth << "x" << hdr.mapHeight << '\n';
    }

    void ScenarioReader::print(PlayerProperties& props) const {
        if (props.hasStartLocation) {
            std::cout
                << "Start Location: "
                << props.startLocX
                << ", "
                << props.startLocY
                << '\n';
        }
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

    void ScenarioReader::print(AiSetting& settings) const {
        std::cout
            << "Count: " << static_cast<int>(settings.amount) << '\n'
            << "Flag:  " << static_cast<int>(settings.flag) << '\n';
    }

    void ScenarioReader::print(ObjectPlacement& obj) const {
        std::cout
            << "Type:    " << static_cast<int>(obj.type) << '\n'
            << "Variant: " << static_cast<int>(obj.variant) << '\n'
            << "X:       " << obj.x << '\n'
            << "Y:       " << obj.y << '\n';
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

    void ScenarioReader::print(GoalLocation& goalLoc) const {
        std::cout
            << "Type: " << static_cast<int>(goalLoc.type) << '\n'
            << "X:    " << static_cast<int>(goalLoc.x) << '\n'
            << "Y:    " << static_cast<int>(goalLoc.y) << '\n';
    }

    void ScenarioReader::print(CampaignText& text) const {
        std::cout
            << "Title:      " << text.title << '\n'
            << "Objectives: " << text.objectives << '\n'
            << "Narration:  " << text.narration << '\n';
    }

}
