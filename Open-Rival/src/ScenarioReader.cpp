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

    ScenarioReader::ScenarioReader(const std::string filename) {

        try {
            readFile(filename);
        } catch (const std::exception& e) {
            std::cerr << e.what();
            throw std::runtime_error("Failed to read scenario: " + filename);
        }
    }

    void ScenarioReader::readFile(const std::string filename) {

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
        for (int i = 0; i < numPlayers; i++) {
            scenarioFile.playerProperties[i] = parsePlayerProperties(data);
        }
        print(scenarioFile.playerProperties[0]);

        // UNKNOWN
        printSection("Skipping unknown section");
        printOffset();
        skip(data, 909, true);

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
        scenarioFile.hireTroopsRestrictions =
                parseHireTroopsRestrictions(data);
        print(scenarioFile.hireTroopsRestrictions);

        // Parse AI building settings
        printSection("Parsing AI building settings");
        printOffset();
        for (int i = 0; i < numBuildingsPerRace; i++) {
            for (int j = 0; j < numAiStrategies; j++) {
                scenarioFile.aiStrategies[j].aiBuildingSettings[i] =
                        parseAiSetting(data);
            }
        }

        // Parse AI troop settings
        printSection("Parsing AI troop settings");
        printOffset();
        for (int i = 0; i < numTroopsPerRace; i++) {
            for (int j = 0; j < numAiStrategies; j++) {
                scenarioFile.aiStrategies[j].aiTroopSettings[i] =
                        parseAiSetting(data);
            }
        }

        // UNKNOWN
        printSection("Skipping unknown section");
        printOffset();
        skip(data, 392, true);

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
                data, scenarioFile.hdr.mapWidth, scenarioFile.hdr.mapHeight);

        // Parse objects
        printSection("Parsing objects");
        printOffset();
        scenarioFile.objects = parseObjects(data);
        std::cout << "Found "
            << scenarioFile.objects.size() << " object(s)\n";
        if (scenarioFile.objects.size() > 0) {
            print(scenarioFile.objects[0]);
        }

        // Parse buildings
        printSection("Parsing buildings");
        printOffset();
        scenarioFile.buildings = parseBuildings(data);
        std::cout << "Found "
                << scenarioFile.buildings.size() << " building(s)\n";
        if (scenarioFile.buildings.size() > 0) {
            print(scenarioFile.buildings[0]);
        }

        // Parse units
        printSection("Parsing units");
        printOffset();
        scenarioFile.units = parseUnits(data);
        std::cout << "Found "
                << scenarioFile.units.size() << " unit(s)\n";
        if (scenarioFile.units.size() > 0) {
            print(scenarioFile.units[0]);
        }

        // Parse chests
        printSection("Parsing chests");
        printOffset();
        scenarioFile.chests = parseChests(data);
        std::cout << "Found "
            << scenarioFile.chests.size() << " chest(s)\n";
        if (scenarioFile.chests.size() > 0) {
            //print(scenarioFile.chests[0]);
        }

        // Parse info points
        printSection("Parsing info points");
        printOffset();
        scenarioFile.infoPoints = parseInfoPoints(data);
        std::cout << "Found "
            << scenarioFile.infoPoints.size() << " info point(s)\n";
        if (scenarioFile.infoPoints.size() > 0) {
            //print(scenarioFile.infoPoints[0]);
        }

        // Parse traps
        printSection("Parsing traps");
        printOffset();
        scenarioFile.traps = parseTraps(data);
        std::cout << "Found "
                << scenarioFile.traps.size() << " trap(s)\n";
        if (scenarioFile.traps.size() > 0) {
            print(scenarioFile.traps[0]);
        }
        skip(data, 8, true);

        // Parse goal locations
        printSection("Parsing goal locations");
        printOffset();
        scenarioFile.goalLocations = parseGoalLocations(data);

        // Parse scenario goals
        printSection("Parsing scenario goals");
        printOffset();
        scenarioFile.goals = parseGoals(data);

        // Parse campaign texts
        printSection("Parsing campaign texts");
        scenarioFile.campaignText = parseCampaignText(data);
        print(scenarioFile.campaignText);
        printOffset();

        // Parse checksum
        printSection("Parsing checksum");
        printOffset();
        scenarioFile.checksum = readByte(data);

        // Check remaining bytes
        size_t remainingBytes = data.size() - pos;
        if (remainingBytes == 0) {
            std::cout << "Reached end of file\n";
        } else {
            std::cout << "Did not reach end of file\n";
            size_t remainingBytesCapped =
                    std::min(remainingBytes, static_cast<size_t>(256));
            printNext(data, remainingBytesCapped);
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Parsing
    ///////////////////////////////////////////////////////////////////////////

    ScenarioHeader ScenarioReader::parseHeader(
            std::vector<unsigned char>& data) {

        ScenarioHeader hdr;

        skip(data, 8, true);
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

        props.hasStartLocation = (readInt(data) == 1);
        props.startLocX = readInt(data);
        props.startLocY = readInt(data);
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

    AiSetting ScenarioReader::parseAiSetting(
            std::vector<unsigned char>& data) {
        AiSetting setting;
        setting.amount = readByte(data);
        setting.flag = readByte(data);
        return setting;
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

    std::vector<ObjectPlacement> ScenarioReader::parseObjects(
            std::vector<unsigned char>& data) {

        std::uint32_t numObjects = readInt(data);
        std::vector<ObjectPlacement> objects;
        objects.reserve(numObjects);

        for (unsigned int i = 0; i < numObjects; i++) {
            ObjectPlacement obj = parseObject(data);
            objects.push_back(obj);
        }

        return objects;
    }

    ObjectPlacement ScenarioReader::parseObject(
            std::vector<unsigned char>& data) {

        ObjectPlacement obj;

        obj.type = readByte(data);
        skip(data, 1, false);
        obj.variant = readByte(data);
        obj.x = readInt(data);
        obj.y = readInt(data);

        return obj;
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
        skip(data, 1, true);
        skip(data, 1, true);
        skip(data, 1, true);
        bldg.x = readShort(data);
        bldg.y = readShort(data);
        skip(data, 1, true);
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
        skip(data, 1, false);

        return bldg;
    }

    std::vector<UnitPlacement> ScenarioReader::parseUnits(
        std::vector<unsigned char>& data) {

        std::uint32_t numUnits = readInt(data);
        std::vector<UnitPlacement> units;
        units.reserve(numUnits);

        for (unsigned int i = 0; i < numUnits; i++) {
            UnitPlacement unit = parseUnit(data);
            units.push_back(unit);
        }

        return units;
    }

    UnitPlacement ScenarioReader::parseUnit(std::vector<unsigned char>& data) {

        UnitPlacement unit;

        unit.type = readByte(data);
        skip(data, 2, true);
        unit.facing = readByte(data);
        skip(data, 1, true);
        unit.x = readShort(data);
        unit.y = readShort(data);
        unit.player = readByte(data);
        unit.hitpoints = readShort(data);
        unit.magic = readByte(data);
        unit.armour = readShort(data);
        skip(data, 1, true);
        unit.type2 = readByte(data);
        unit.sight = readByte(data);
        unit.range = readByte(data);
        skip(data, 2, true);
        unit.specialColour = readByte(data);
        unit.prisoner = readBool(data);
        unit.goldCost = readShort(data);
        unit.woodCost = readShort(data);
        unit.name = readString(data, 12);
        skip(data, 13, true);
        unit.upgrade1Enabled = readBool(data);
        unit.upgrade2Enabled = readBool(data);
        unit.upgrade3Enabled = readBool(data);
        unit.upgrade4Enabled = readBool(data);
        unit.fightingArea = readByte(data);

        return unit;
    }

    std::vector<ChestPlacement> ScenarioReader::parseChests(
            std::vector<unsigned char>& data) {

        std::uint32_t numChests = readInt(data);
        std::vector<ChestPlacement> chests;
        chests.reserve(numChests);

        for (unsigned int i = 0; i < numChests; i++) {
            ChestPlacement chest = parseChest(data);
            chests.push_back(chest);
        }

        return chests;
    }

    ChestPlacement ScenarioReader::parseChest(
            std::vector<unsigned char>& data) {

        ChestPlacement obj;

        obj.type = readInt(data);
        obj.variant = readInt(data);
        obj.x = readByte(data);
        obj.y = readByte(data);
        skip(data, 10, true); // items

        return obj;
    }

    std::vector<InfoPointPlacement> ScenarioReader::parseInfoPoints(
            std::vector<unsigned char>& data) {

        std::uint32_t numInfoPoints = readInt(data);
        std::vector<InfoPointPlacement> infoPoints;
        infoPoints.reserve(numInfoPoints);

        for (unsigned int i = 0; i < numInfoPoints; i++) {
            InfoPointPlacement infoPoint = parseInfoPoint(data);
            infoPoints.push_back(infoPoint);
        }

        return infoPoints;
    }

    InfoPointPlacement ScenarioReader::parseInfoPoint(
            std::vector<unsigned char>& data) {

        InfoPointPlacement infoPoint;

        infoPoint.x = readByte(data);
        infoPoint.y = readByte(data);
        std::uint16_t messageLength = readShort(data);
        infoPoint.message = readString(data, messageLength);

        return infoPoint;
    }

    std::vector<TrapPlacement> ScenarioReader::parseTraps(
            std::vector<unsigned char>& data) {

        std::uint32_t numTraps = readInt(data);
        std::vector<TrapPlacement> traps;
        traps.reserve(numTraps);

        for (unsigned int i = 0; i < numTraps; i++) {
            TrapPlacement trap = parseTrap(data);
            traps.push_back(trap);
        }

        return traps;
    }

    TrapPlacement ScenarioReader::parseTrap(
            std::vector<unsigned char>& data) {

        TrapPlacement trap;

        trap.x = readByte(data);
        trap.y = readByte(data);
        trap.player = readByte(data);

        return trap;
    }

    std::vector<GoalLocation> ScenarioReader::parseGoalLocations(
            std::vector<unsigned char>& data) {

        std::uint8_t numGoals = readRivalByte(data);
        std::vector<GoalLocation> goals;
        goals.reserve(numGoals);

        for (unsigned int i = 0; i < numGoals; i++) {
            GoalLocation goal = parseGoalLocation(data);
            goals.push_back(goal);
        }

        return goals;
    }

    GoalLocation ScenarioReader::parseGoalLocation(
            std::vector<unsigned char>& data) {

        GoalLocation goal;

        goal.type = readRivalByte(data);
        goal.x = readRivalByte(data);
        skip(data, 3, false);
        goal.y = readRivalByte(data);
        skip(data, 3, false);

        return goal;
    }

    std::vector<Goal> ScenarioReader::parseGoals(
            std::vector<unsigned char>& data) {

        std::uint8_t numGoals = readRivalByte(data);
        skip(data, 3, false);
        std::vector<Goal> goals;
        goals.reserve(numGoals);

        for (unsigned int i = 0; i < numGoals; i++) {
            Goal goal = parseGoal(data);
            goals.push_back(goal);
        }

        return goals;
    }

    Goal ScenarioReader::parseGoal(std::vector<unsigned char>& data) {

        Goal goal;

        std::uint8_t goalType = readByte(data);
        skip(data, 11, true);

        // TODO: parse goal based on type
        goal.count = 0;

        return goal;
    }

    CampaignText ScenarioReader::parseCampaignText(
            std::vector<unsigned char>& data) {

        CampaignText text;

        std::uint8_t titleLength = readRivalByte(data);
        if (titleLength > 0) {
            skip(data, 1, true);
            text.title = readRivalString(data, titleLength);
        }

        std::uint8_t objectivesLength = readRivalByte(data);
        if (objectivesLength > 0) {
            skip(data, 1, true);
            text.objectives = readRivalString(data, objectivesLength);
        }

        std::uint8_t narrationLength = readRivalByte(data);
        if (narrationLength > 0) {
            skip(data, 1, true);
            text.narration = readRivalString(data, narrationLength);
        }

        return text;
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
        return std::uint8_t(data[offset]);
    }

    std::uint8_t ScenarioReader::readRivalByte(
            std::vector<unsigned char>& data) {
        std::uint8_t value = readRivalByte(data, pos);
        pos += 1;
        return value;
    }

    // Special numbering used by goals, etc.
    std::uint8_t ScenarioReader::readRivalByte(
            std::vector<unsigned char>& data, size_t offset) const {
        std::uint8_t val = std::uint8_t(data[offset]);

        // Values in this format are offset by 0x74 or 0x70, depending on
        // the value of the '2' column in the binary representation
        if ((val & 0x02) > 0) {
            return val - 0x70;
        } else {
            return val - 0x74;
        }
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

        std::vector<char> chars(length);
        for (size_t i = 0; i < length; i++) {
            chars[i] = data[offset + i];
        }
        std::string value(chars.data(), length);
        return value;
    }

    std::string ScenarioReader::readRivalString(
            std::vector<unsigned char>& data, size_t length) {
        std::string value = readRivalString(data, pos, length);
        pos += length;
        return value;
    }

    std::string ScenarioReader::readRivalString(
            std::vector<unsigned char>& data,
            size_t offset,
            size_t length) const {

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

    void ScenarioReader::print(CampaignText& text) const {
        std::cout
            << "Title:      " << text.title << '\n'
            << "Objectives: " << text.objectives << '\n'
            << "Narration:  " << text.narration << '\n';
    }

}
