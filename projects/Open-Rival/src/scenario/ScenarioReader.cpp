#include "scenario/ScenarioReader.h"

#include <algorithm>
#include <iomanip>  // std::setfill, std::setw
#include <ios>      // std::hex
#include <stdexcept>

#include "entity/Entity.h"
#include "utils/FileUtils.h"
#include "utils/LogUtils.h"

namespace Rival {

/* clang-format off */
    const std::unordered_map<std::uint8_t, char> ScenarioReader::alphabet = {
        { std::uint8_t(0x12), 'b' },
        { std::uint8_t(0x13), 'c' },
        { std::uint8_t(0x15), 'a' },
        { std::uint8_t(0x16), 'f' },
        { std::uint8_t(0x17), 'g' },
        { std::uint8_t(0x18), 'd' },
        { std::uint8_t(0x19), 'e' },
        { std::uint8_t(0x1A), 'j' },
        { std::uint8_t(0x1B), 'k' },
        { std::uint8_t(0x1C), 'h' },
        { std::uint8_t(0x1D), 'i' },
        { std::uint8_t(0x1E), 'n' },
        { std::uint8_t(0x1F), 'o' },
        { std::uint8_t(0x20), 'l' },
        { std::uint8_t(0x21), 'm' },
        { std::uint8_t(0x22), 'r' },
        { std::uint8_t(0x23), 's' },
        { std::uint8_t(0x24), 'p' },
        { std::uint8_t(0x26), 'v' },
        { std::uint8_t(0x27), 'w' },
        { std::uint8_t(0x28), 't' },
        { std::uint8_t(0x29), 'u' },
        { std::uint8_t(0x2A), 'z' },
        { std::uint8_t(0x2C), 'x' },
        { std::uint8_t(0x2D), 'y' },
        { std::uint8_t(0x32), 'B' },
        { std::uint8_t(0x33), 'C' },
        { std::uint8_t(0x35), 'A' },
        { std::uint8_t(0x36), 'F' },
        { std::uint8_t(0x37), 'G' },
        { std::uint8_t(0x38), 'D' },
        { std::uint8_t(0x39), 'E' },
        { std::uint8_t(0x3A), 'J' },
        { std::uint8_t(0x3B), 'K' },
        { std::uint8_t(0x3C), 'H' },
        { std::uint8_t(0x3D), 'I' },
        { std::uint8_t(0x3E), 'N' },
        { std::uint8_t(0x3F), 'O' },
        { std::uint8_t(0x40), 'L' },
        { std::uint8_t(0x41), 'M' },
        { std::uint8_t(0x42), 'R' },
        { std::uint8_t(0x43), 'S' },
        { std::uint8_t(0x44), 'P' },
        { std::uint8_t(0x46), 'V' },
        { std::uint8_t(0x47), 'W' },
        { std::uint8_t(0x48), 'T' },
        { std::uint8_t(0x49), 'U' },
        { std::uint8_t(0x4A), 'Z' },
        { std::uint8_t(0x4C), 'X' },
        { std::uint8_t(0x4D), 'Y' },
        { std::uint8_t(0x52), '"' },
        { std::uint8_t(0x54), ' ' },
        { std::uint8_t(0x56), '&' },
        { std::uint8_t(0x60), ',' },
        { std::uint8_t(0x61), '-' },
        { std::uint8_t(0x62), '2' },
        { std::uint8_t(0x63), '3' },
        { std::uint8_t(0x71), '=' },
        { std::uint8_t(0x81), '\r' },
        { std::uint8_t(0x7A), '\n' }
    };
/* clang-format on */

///////////////////////////////////////////////////////////////////////////
// Entry points
///////////////////////////////////////////////////////////////////////////

ScenarioReader::ScenarioReader(const std::string& filename)
{
    try
    {
        data = FileUtils::readBinaryFile(filename);
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("{}", e.what());
        throw std::runtime_error("Failed to read scenario: " + filename);
    }
}

ScenarioData ScenarioReader::readScenario()
{
    pos = 0;
    ScenarioData scenarioData = parseScenario(true);
    LOG_TRACE("Scenario data:\n{}", log.view());
    return scenarioData;
}

ScenarioData ScenarioReader::readCampaignScenario(int levelIndex)
{
    pos = 0;
    /*std::uint8_t numLevels = */ readByte();

    // Skip entries for all scenarios before this one
    for (int i = 0; i < levelIndex; ++i)
    {
        skip(8, false);
    }

    // Read this scenario's offset and size
    std::uint32_t offset = readInt();
    /*std::uint32_t size =*/readInt();

    // Parse the scenario
    pos = offset;
    ScenarioData scenarioData = parseScenario(false);
    return scenarioData;
}

///////////////////////////////////////////////////////////////////////////
// Parsing
///////////////////////////////////////////////////////////////////////////

ScenarioData ScenarioReader::parseScenario(bool expectEof)
{
    ScenarioData scenarioData;

    // Parse map header
    printSection("Parsing header");
    printOffset();
    scenarioData.hdr = parseHeader();
    print(scenarioData.hdr);

    // Parse player properties
    printSection("Parsing player properties");
    printOffset();
    for (int i = 0; i < ScenarioConstants::numPlayers; ++i)
    {
        scenarioData.playerProperties[i] = parsePlayerProperties();
    }
    print(scenarioData.playerProperties[0]);

    // UNKNOWN
    printSection("Skipping unknown section");
    printOffset();
    skip(909, false);

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
    for (int i = 0; i < ScenarioConstants::numTroops; ++i)
    {
        scenarioData.troopDefaults[i] = parseTroopDefaults();
    }
    print(scenarioData.troopDefaults[0]);
    skip(7, false);

    // Parse upgrade properties
    printSection("Parsing upgrade properties");
    printOffset();
    for (int i = 0; i < ScenarioConstants::numUpgrades; ++i)
    {
        scenarioData.upgradeProperties[i] = parseUpgradeProperties(doesUpgradeHaveAmount(i));
    }
    print(scenarioData.upgradeProperties[0]);

    // UNKNOWN
    printSection("Skipping unknown section");
    printOffset();
    skip(176, false);

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
    for (int i = 0; i < ScenarioConstants::numProductionCosts; ++i)
    {
        scenarioData.productionCosts[i] = parseProductionCost();
    }
    print(scenarioData.productionCosts[0]);
    skip(27, false);

    // Parse weapon defaults
    printSection("Parsing weapon defaults");
    printOffset();
    for (int i = 0; i < ScenarioConstants::numWeapons; ++i)
    {
        scenarioData.weaponDefaults[i] = parseWeaponDefaults();
    }
    print(scenarioData.weaponDefaults[0]);
    skip(2, false);

    // Parse available buildings
    printSection("Parsing available buildings");
    printOffset();
    scenarioData.availableBuildings = parseAvailableBuildings();
    print(scenarioData.availableBuildings);
    skip(5, false);

    // Parse monster defaults
    printSection("Parsing monster defaults");
    printOffset();
    for (int i = 0; i < ScenarioConstants::numMonsters; ++i)
    {
        scenarioData.monsterDefaults[i] = parseTroopDefaults();
    }
    print(scenarioData.monsterDefaults[0]);
    skip(49, false);

    // Parse hire troops restrictions
    printSection("Parsing hire troops restrictions");
    printOffset();
    scenarioData.hireTroopsRestrictions = parseHireTroopsRestrictions();
    print(scenarioData.hireTroopsRestrictions);

    // Parse AI building settings
    printSection("Parsing AI building settings");
    printOffset();
    for (int i = 0; i < ScenarioConstants::numBuildingsPerRace; ++i)
    {
        for (int j = 0; j < ScenarioConstants::numAiStrategies; ++j)
        {
            scenarioData.aiStrategies[j].aiBuildingSettings[i] = parseAiSetting();
        }
    }

    // Parse AI troop settings
    printSection("Parsing AI troop settings");
    printOffset();
    for (int i = 0; i < ScenarioConstants::numTroopsPerRace; ++i)
    {
        for (int j = 0; j < ScenarioConstants::numAiStrategies; ++j)
        {
            scenarioData.aiStrategies[j].aiTroopSettings[i] = parseAiSetting();
        }
    }

    // UNKNOWN
    printSection("Skipping unknown section");
    printOffset();
    skip(392, false);

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
    scenarioData.tiles = parseTiles(scenarioData.hdr.mapWidth, scenarioData.hdr.mapHeight);

    // Parse objects
    printSection("Parsing objects");
    printOffset();
    scenarioData.objects = parseObjects();
    log << "Found " << scenarioData.objects.size() << " object(s)\n";
    if (scenarioData.objects.size() > 0)
    {
        print(scenarioData.objects[0]);
    }

    // Parse buildings
    printSection("Parsing buildings");
    printOffset();
    scenarioData.buildings = parseBuildings();
    log << "Found " << scenarioData.buildings.size() << " building(s)\n";
    if (scenarioData.buildings.size() > 0)
    {
        print(scenarioData.buildings[0]);
    }

    // Parse units
    printSection("Parsing units");
    printOffset();
    scenarioData.units = parseUnits();
    log << "Found " << scenarioData.units.size() << " unit(s)\n";
    if (scenarioData.units.size() > 0)
    {
        print(scenarioData.units[0]);
    }

    // Parse chests
    printSection("Parsing chests");
    printOffset();
    scenarioData.chests = parseChests();
    log << "Found " << scenarioData.chests.size() << " chest(s)\n";
    if (scenarioData.chests.size() > 0)
    {
        // print(scenarioData.chests[0]);
    }

    // Parse info points
    printSection("Parsing info points");
    printOffset();
    scenarioData.infoPoints = parseInfoPoints();
    log << "Found " << scenarioData.infoPoints.size() << " info point(s)\n";
    if (scenarioData.infoPoints.size() > 0)
    {
        // print(scenarioData.infoPoints[0]);
    }

    // Parse traps
    printSection("Parsing traps");
    printOffset();
    scenarioData.traps = parseTraps();
    log << "Found " << scenarioData.traps.size() << " trap(s)\n";
    if (scenarioData.traps.size() > 0)
    {
        print(scenarioData.traps[0]);
    }
    skip(8, false);

    // Parse goal locations
    printSection("Parsing goal locations");
    printOffset();
    scenarioData.goalLocations = parseGoalLocations();
    log << "Found " << scenarioData.goalLocations.size() << " goal location(s)\n";
    if (scenarioData.goalLocations.size() > 0)
    {
        print(scenarioData.goalLocations[0]);
    }

    // Parse scenario goals
    printSection("Parsing scenario goals");
    printOffset();
    scenarioData.goals = parseGoals();

    // Parse campaign texts
    printSection("Parsing campaign texts");
    printOffset();
    scenarioData.campaignText = parseCampaignText();
    print(scenarioData.campaignText);
    printOffset();

    // Parse checksum
    printSection("Parsing checksum");
    printOffset();
    scenarioData.checksum = readByte();

    // Check remaining bytes
    if (expectEof)
    {
        std::size_t remainingBytes = data.size() - pos;
        if (remainingBytes == 0)
        {
            log << "Reached end of file\n";
        }
        else
        {
            log << "Found unexpected bytes:\n";
            std::size_t remainingBytesCapped = std::min(remainingBytes, static_cast<std::size_t>(256));
            printNext(remainingBytesCapped);
            throw std::runtime_error("Did not reach end of file");
        }
    }

    return scenarioData;
}

ScenarioHeader ScenarioReader::parseHeader()
{
    ScenarioHeader hdr;

    skip(8, false);
    std::uint8_t terrainType = readByte();
    hdr.wilderness = (terrainType == 0x03);
    std::uint8_t nameLength = readByte();
    hdr.mapName = readString(nameLength);
    hdr.mapHeight = readInt();
    hdr.mapWidth = readInt();
    skip(9, false);

    return hdr;
}

PlayerProperties ScenarioReader::parsePlayerProperties()
{
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

TroopDefaults ScenarioReader::parseTroopDefaults()
{
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

bool ScenarioReader::doesUpgradeHaveAmount(int i) const
{
    // Upgrades 0 - 47 and 67 - 69 do not have an "Amount" field
    return i <= 47 || (i >= 67 && i <= 69);
}

UpgradeProperties ScenarioReader::parseUpgradeProperties(bool readAmount)
{

    UpgradeProperties upgrade;

    if (readAmount)
    {
        upgrade.amount = readInt();
    }
    else
    {
        upgrade.amount = 0;
        skip(4, false);
    }
    upgrade.goldCost = readInt();
    upgrade.woodCost = readInt();
    upgrade.unknown = readInt();

    return upgrade;
}

ProductionCost ScenarioReader::parseProductionCost()
{
    ProductionCost cost;

    cost.goldCost = readShort();
    cost.woodCost = readShort();
    cost.constructionTime = readInt();
    cost.requiredExpOrIncreasePercent = readInt();

    return cost;
}

WeaponDefaults ScenarioReader::parseWeaponDefaults()
{
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

AvailableBuildings ScenarioReader::parseAvailableBuildings()
{
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

HireTroopsRestrictions ScenarioReader::parseHireTroopsRestrictions()
{
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

AiSetting ScenarioReader::parseAiSetting()
{
    AiSetting setting;
    setting.amount = readByte();
    setting.flag = readByte();
    return setting;
}

std::vector<TilePlacement> ScenarioReader::parseTiles(int width, int height)
{

    unsigned int numTiles = width * height;
    std::vector<TilePlacement> tiles;
    tiles.reserve(numTiles);
    std::size_t tileId = 0;

    while (tileId < numTiles)
    {

        // Determine how many tiles the next tile definition describes,
        // and skip to the tile definition
        bool readMultiple = (readByte() == 0xfa);
        std::uint16_t numUpcomingTiles = 1;
        if (readMultiple)
        {
            numUpcomingTiles = readShort();
            skip(4, false);
        }
        else
        {
            skip(3, false);
        }

        // Read the tile definition
        TilePlacement tile = parseTile();

        // Copy the tile definition to the number of tiles it describes
        for (std::size_t i = 0; i < numUpcomingTiles; ++i)
        {
            tiles.push_back(tile);
            tileId++;
        }

        pos += bytesPerTile;
    }

    return tiles;
}

TilePlacement ScenarioReader::parseTile()
{
    TilePlacement tile;

    tile.resource = readByte(pos);
    tile.type = readByte(pos + 2);
    tile.variant = readByte(pos + 4);

    return tile;
}

std::vector<ObjectPlacement> ScenarioReader::parseObjects()
{
    std::uint32_t numObjects = readInt();
    std::vector<ObjectPlacement> objects;
    objects.reserve(numObjects);

    for (unsigned int i = 0; i < numObjects; ++i)
    {
        ObjectPlacement obj = parseObject();
        objects.push_back(obj);
    }

    return objects;
}

ObjectPlacement ScenarioReader::parseObject()
{
    ObjectPlacement obj;

    obj.type = readByte();
    skip(1, false);
    obj.variant = readByte();
    obj.x = readInt();
    obj.y = readInt();

    return obj;
}

std::vector<BuildingPlacement> ScenarioReader::parseBuildings()
{
    std::uint32_t numBuildings = readInt();
    std::vector<BuildingPlacement> buildings;
    buildings.reserve(numBuildings);

    for (unsigned int i = 0; i < numBuildings; ++i)
    {
        BuildingPlacement bldg = parseBuilding();
        buildings.push_back(bldg);
    }

    return buildings;
}

BuildingPlacement ScenarioReader::parseBuilding()
{
    BuildingPlacement bldg;

    bldg.type = readByte();
    skip(1, false);
    bldg.wallVariant = readByte();
    skip(1, false);
    skip(1, false);
    bldg.x = readShort();
    bldg.y = readShort();
    bldg.player = readByte();
    bldg.hitpoints = readShort();
    bldg.armour = readShort();
    skip(1, false);
    bldg.sight = readByte();
    bldg.range = readByte();
    bldg.upgrade1Enabled = readBool();
    bldg.upgrade2Enabled = readBool();
    bldg.specialColor = readByte();
    bldg.prisoner = readBool();
    bldg.name = readString(12);
    skip(1, false);

    return bldg;
}

std::vector<UnitPlacement> ScenarioReader::parseUnits()
{
    std::uint32_t numUnits = readInt();
    std::vector<UnitPlacement> units;
    units.reserve(numUnits);

    for (unsigned int i = 0; i < numUnits; ++i)
    {
        UnitPlacement unit = parseUnit();
        units.push_back(unit);
    }

    return units;
}

UnitPlacement ScenarioReader::parseUnit()
{
    UnitPlacement unit;

    unit.type = readByte();
    skip(2, false);
    unit.facing = readByte();
    skip(1, false);
    unit.x = readShort();
    unit.y = readShort();
    unit.player = readByte();
    unit.hitpoints = readShort();
    unit.magic = readByte();
    unit.armour = readShort();
    skip(1, false);
    unit.type2 = readByte();
    unit.sight = readByte();
    unit.range = readByte();
    skip(2, false);
    unit.specialColor = readByte();
    unit.prisoner = readBool();
    unit.goldCost = readShort();
    unit.woodCost = readShort();
    unit.name = readString(Entity::maxNameLength);
    skip(13, false);
    unit.upgrade1Enabled = readBool();
    unit.upgrade2Enabled = readBool();
    unit.upgrade3Enabled = readBool();
    unit.upgrade4Enabled = readBool();
    unit.fightingArea = readByte();

    return unit;
}

std::vector<ChestPlacement> ScenarioReader::parseChests()
{
    std::uint32_t numChests = readInt();
    std::vector<ChestPlacement> chests;
    chests.reserve(numChests);

    for (unsigned int i = 0; i < numChests; ++i)
    {
        ChestPlacement chest = parseChest();
        chests.push_back(chest);
    }

    return chests;
}

ChestPlacement ScenarioReader::parseChest()
{
    ChestPlacement obj;

    obj.type = readInt();
    obj.variant = readInt();
    obj.x = readByte();
    obj.y = readByte();
    skip(10, false);  // items

    return obj;
}

std::vector<InfoPointPlacement> ScenarioReader::parseInfoPoints()
{
    std::uint32_t numInfoPoints = readInt();
    std::vector<InfoPointPlacement> infoPoints;
    infoPoints.reserve(numInfoPoints);

    for (unsigned int i = 0; i < numInfoPoints; ++i)
    {
        InfoPointPlacement infoPoint = parseInfoPoint();
        infoPoints.push_back(infoPoint);
    }

    return infoPoints;
}

InfoPointPlacement ScenarioReader::parseInfoPoint()
{
    InfoPointPlacement infoPoint;

    infoPoint.x = readByte();
    infoPoint.y = readByte();
    std::uint16_t messageLength = readShort();
    infoPoint.message = readString(messageLength);

    return infoPoint;
}

std::vector<TrapPlacement> ScenarioReader::parseTraps()
{
    std::uint32_t numTraps = readInt();
    std::vector<TrapPlacement> traps;
    traps.reserve(numTraps);

    for (unsigned int i = 0; i < numTraps; ++i)
    {
        TrapPlacement trap = parseTrap();
        traps.push_back(trap);
    }

    return traps;
}

TrapPlacement ScenarioReader::parseTrap()
{
    TrapPlacement trap;

    trap.x = readByte();
    trap.y = readByte();
    trap.player = readByte();

    return trap;
}

std::vector<GoalLocation> ScenarioReader::parseGoalLocations()
{
    std::uint8_t numGoalLocations = readRivalByte();
    std::vector<GoalLocation> goals;
    goals.reserve(numGoalLocations);

    for (unsigned int i = 0; i < numGoalLocations; ++i)
    {
        GoalLocation goal = parseGoalLocation();
        goals.push_back(goal);
    }

    return goals;
}

GoalLocation ScenarioReader::parseGoalLocation()
{
    GoalLocation goal;

    goal.type = readRivalByte();
    goal.x = readRivalByte();
    skip(3, false);
    goal.y = readRivalByte();
    skip(3, false);

    return goal;
}

std::vector<Goal> ScenarioReader::parseGoals()
{
    std::uint8_t numGoals = readRivalByte();

    if (numGoals > 0)
    {
        skip(3, false);
    }

    std::vector<Goal> goals;
    goals.reserve(numGoals);

    for (unsigned int i = 0; i < numGoals; ++i)
    {
        Goal goal = parseGoal();
        goals.push_back(goal);
    }

    return goals;
}

Goal ScenarioReader::parseGoal()
{
    Goal goal;

    /*std::uint8_t goalType = */ readByte();
    skip(11, false);

    // TODO: parse goal based on type
    goal.count = 0;

    return goal;
}

CampaignText ScenarioReader::parseCampaignText()
{
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

std::uint8_t ScenarioReader::readByte()
{
    std::uint8_t value = readByte(pos);
    pos += 1;
    return value;
}

std::uint8_t ScenarioReader::readByte(std::size_t offset) const
{
    return std::uint8_t(data[offset]);
}

std::uint8_t ScenarioReader::readRivalByte()
{
    std::uint8_t value = readRivalByte(pos);
    pos += 1;
    return value;
}

// Special numbering used by goals, etc.
std::uint8_t ScenarioReader::readRivalByte(std::size_t offset) const
{
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
std::uint8_t ScenarioReader::fixRivalByte(std::uint8_t val) const
{
    // Apply tiered offset
    if (val < 0x12)
    {
        val += 0x8C;
    }
    else if (val < 0x32)
    {
        val += 0x4C;
    }
    else if (val < 0x52)
    {
        val -= 0x14;
    }
    else if (val < 0x72)
    {
        val -= 0x34;
    }
    else if (val < 0x92)
    {
        val -= 0x74;
    }
    else if (val < 0xB2)
    {
        val += 0x4C;
    }
    else if (val < 0xD2)
    {
        val += 0x0C;
    }
    else if (val < 0xF2)
    {
        val -= 0x34;
    }
    else
    {
        val -= 0x74;
    }

    // Apply a further offset to every other pair of values
    if ((val & 0x02) > 0)
    {
        val += 0x04;
    }

    return val;
}

std::uint16_t ScenarioReader::readRivalShort()
{
    std::uint16_t value = readRivalShort(pos);
    pos += numBytesShort;
    return value;
}

std::uint16_t ScenarioReader::readRivalShort(std::size_t offset) const
{
    // read 2 rival bytes, and combine them like a normal short
    return std::uint16_t(fixRivalByte(data[offset + 1]) << 8 | fixRivalByte(data[offset + 0]));
}

bool ScenarioReader::readBool()
{
    bool value = readBool(pos);
    pos += 1;
    return value;
}

bool ScenarioReader::readBool(std::size_t offset) const
{
    std::uint8_t value = data[offset];
    return value == 1;
}

std::uint16_t ScenarioReader::readShort()
{
    std::uint16_t value = readShort(pos);
    pos += numBytesShort;
    return value;
}

std::uint16_t ScenarioReader::readShort(std::size_t offset) const
{
    // little endian
    return std::uint16_t(data[offset + 1] << 8 | data[offset + 0]);
}

std::uint32_t ScenarioReader::readInt()
{
    std::uint32_t value = readInt(pos);
    pos += numBytesInt;
    return value;
}

std::uint32_t ScenarioReader::readInt(std::size_t offset) const
{
    // little endian
    return std::uint32_t(data[offset + 3] << 24  //
            | data[offset + 2] << 16             //
            | data[offset + 1] << 8              //
            | data[offset + 0]);
}

std::string ScenarioReader::readString(std::size_t length)
{
    std::string value = readString(pos, length);
    pos += length;
    return value;
}

std::string ScenarioReader::readString(std::size_t offset, std::size_t length) const
{
    std::vector<char> chars(length);
    for (std::size_t i = 0; i < length; ++i)
    {
        chars[i] = data[offset + i];
    }
    std::string value(chars.data(), length);
    return value;
}

std::string ScenarioReader::readRivalString(std::size_t length)
{
    std::string value = readRivalString(pos, length);
    pos += length;
    return value;
}

std::string ScenarioReader::readRivalString(std::size_t offset, std::size_t length) const
{

    std::vector<char> chars(length);
    for (std::size_t i = 0; i < length; ++i)
    {
        std::uint8_t c = data[offset + i];
        chars[i] = getRivalChar(c);
    }
    std::string value(chars.data(), length);
    return value;
}

char ScenarioReader::getRivalChar(std::uint8_t c) const
{

    if (c == 0x2B || c == 0x31)
    {
        // Special value to signify red text
        return 0x02;
    }
    if (c == 0x31)
    {
        // Special value to signify the end of red text
        return 0x03;
    }

    const auto it = alphabet.find(c);
    if (it != alphabet.cend())
    {
        // Entry found
        return it->second;
    }

    log << "Found unknown character: " << static_cast<int>(c) << "\n";

    // Unknown character
    return '?';
}

void ScenarioReader::skip(const std::size_t n, bool print)
{
    if (print)
    {
        log << "SKIP: ";
        printNext(n);
    }
    pos += n;
}

///////////////////////////////////////////////////////////////////////////
// Printing
///////////////////////////////////////////////////////////////////////////

void ScenarioReader::printOffset() const
{
    // Switch to hex, print the value, and switch back
    log                           //
            << "Offset: 0x"       //
            << std::setw(4)       //
            << std::setfill('0')  //
            << std::hex           //
            << pos                //
            << '\n'               //
            << std::dec;
}

void ScenarioReader::printSection(std::string title) const
{
    log << "\n==================================================\n"
        << title << '\n'
        << "==================================================\n\n";
}

void ScenarioReader::printNext(const std::size_t n) const
{
    // Switch to hex
    log << std::setfill('0') << std::hex;
    int col = 0;
    for (std::size_t i = 0; i < n; ++i)
    {
        unsigned int value = static_cast<unsigned int>(data.at(pos + i));

        if (value == 0)
        {
            log << "-- ";
        }
        else
        {
            log << std::setw(2) << value << " ";
        }

        // Print a new line every 16 bytes
        col++;
        if (col == 16)
        {
            log << '\n';
            col = 0;
        }
    }
    // Switch back to decimal
    log << '\n' << std::dec;
}

void ScenarioReader::print(const ScenarioHeader& hdr) const
{
    log << "Map Name: " << hdr.mapName << '\n' << "Map Size: " << hdr.mapWidth << "x" << hdr.mapHeight << '\n';
}

void ScenarioReader::print(const PlayerProperties& props) const
{
    if (props.hasStartLocation)
    {
        log << "Start Location: " << props.startLocX << ", " << props.startLocY << '\n';
    }
    log << "Starting Gold:  " << props.startingGold << '\n'
        << "Starting Wood:  " << props.startingWood << '\n'
        << "Starting Food:  " << props.startingFood << '\n'
        << "Race:           " << static_cast<int>(props.race) << '\n'
        << "AI:             " << props.ai << '\n'
        << "AI Type:        " << static_cast<int>(props.aiType) << '\n'
        << "AI Performance: " << static_cast<int>(props.aiPerformance) << '\n'
        << "AI Strategy:    " << static_cast<int>(props.aiStrategy) << '\n';
}

void ScenarioReader::print(const TroopDefaults& troop) const
{
    log << "Hitpoints: " << troop.hitpoints << '\n'
        << "Magic:     " << troop.magic << '\n'
        << "Armour:    " << static_cast<int>(troop.armour) << '\n'
        << "Sight:     " << static_cast<int>(troop.sight) << '\n'
        << "Range:     " << static_cast<int>(troop.range) << '\n';
}

void ScenarioReader::print(const UpgradeProperties& upgrade) const
{
    log << "Amount:    " << upgrade.amount << '\n'
        << "Gold Cost: " << upgrade.goldCost << '\n'
        << "Wood Cost: " << upgrade.woodCost << '\n'
        << "Unknown:   " << upgrade.unknown << '\n';
}

void ScenarioReader::print(const ProductionCost& cost) const
{
    log << "Gold Cost:         " << cost.goldCost << '\n'
        << "Wood Cost:         " << cost.woodCost << '\n'
        << "Construction Time: " << cost.constructionTime << '\n'
        << "XP or Increase:    " << cost.requiredExpOrIncreasePercent << '\n';
}

void ScenarioReader::print(const WeaponDefaults& wpn) const
{
    log << "Move Spaces:  " << wpn.moveSpaces << '\n'
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

void ScenarioReader::print(const AvailableBuildings& bldg) const
{
    log << "Crop Land:                  " << static_cast<int>(bldg.cropLand) << '\n'
        << "Gold Amplifier:             " << static_cast<int>(bldg.goldAmplifier) << '\n'
        << "Ranged Troop Building:      " << static_cast<int>(bldg.rangedTroopBuilding) << '\n'
        << "Siege Troop Building:       " << static_cast<int>(bldg.siegeTroopBuilding) << '\n'
        << "Melee Troop Building:       " << static_cast<int>(bldg.meleeTroopBuilding) << '\n'
        << "Flying Troop Building:      " << static_cast<int>(bldg.flyingTroopBuilding) << '\n'
        << "Engineer Troop Building:    " << static_cast<int>(bldg.engineerTroopBuilding) << '\n'
        << "Healer Troop Building:      " << static_cast<int>(bldg.healerTroopBuilding) << '\n'
        << "Spellcaster Troop Building: " << static_cast<int>(bldg.spellcasterTroopBuilding) << '\n'
        << "Shipyard:                   " << static_cast<int>(bldg.shipyard) << '\n'
        << "Watch Tower:                " << static_cast<int>(bldg.watchTower) << '\n'
        << "Wall:                       " << static_cast<int>(bldg.wall) << '\n';
}

void ScenarioReader::print(const HireTroopsRestrictions& restrictions) const
{
    log << "Worker:                    " << static_cast<int>(restrictions.worker) << '\n'
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

void ScenarioReader::print(const AiSetting& settings) const
{
    log << "Count: " << static_cast<int>(settings.amount) << '\n'
        << "Flag:  " << static_cast<int>(settings.flag) << '\n';
}

void ScenarioReader::print(const ObjectPlacement& obj) const
{
    log << "Type:    " << static_cast<int>(obj.type) << '\n'
        << "Variant: " << static_cast<int>(obj.variant) << '\n'
        << "X:       " << obj.x << '\n'
        << "Y:       " << obj.y << '\n';
}

void ScenarioReader::print(const BuildingPlacement& bldg) const
{
    log << "Type:           " << static_cast<int>(bldg.type) << '\n'
        << "Player:         " << static_cast<int>(bldg.player) << '\n'
        << "X:              " << bldg.x << '\n'
        << "Y:              " << bldg.y << '\n'
        << "Hitpoints:      " << bldg.hitpoints << '\n'
        << "Armour:         " << bldg.armour << '\n'
        << "Sight:          " << static_cast<int>(bldg.sight) << '\n'
        << "Range:          " << static_cast<int>(bldg.range) << '\n'
        << "Upgrade 1:      " << bldg.upgrade1Enabled << '\n'
        << "Upgrade 2:      " << bldg.upgrade2Enabled << '\n'
        << "Special Colour: " << static_cast<int>(bldg.specialColor) << '\n'
        << "Prisoner:       " << bldg.prisoner << '\n'
        << "Name:           " << bldg.name << '\n';
}

void ScenarioReader::print(const UnitPlacement& unit) const
{
    log << "Type:           " << static_cast<int>(unit.type) << '\n'
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
        << "Special Colour: " << static_cast<int>(unit.specialColor) << '\n'
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

void ScenarioReader::print(const TrapPlacement& trap) const
{
    log << "X:              " << static_cast<int>(trap.x) << '\n'
        << "Y:              " << static_cast<int>(trap.y) << '\n'
        << "Player:         " << static_cast<int>(trap.player) << '\n';
}

void ScenarioReader::print(const GoalLocation& goalLoc) const
{
    log << "Type: " << static_cast<int>(goalLoc.type) << '\n'
        << "X:    " << static_cast<int>(goalLoc.x) << '\n'
        << "Y:    " << static_cast<int>(goalLoc.y) << '\n';
}

void ScenarioReader::print(const CampaignText& text) const
{
    log << "Title:      " << text.title << '\n'
        << "Objectives: " << text.objectives << '\n'
        << "Narration:  " << text.narration << '\n';
}

}  // namespace Rival
