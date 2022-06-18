#pragma once

#include <array>
#include <cstdint>
#include <vector>

// This header file contains all the structs used to represent a Scenario file
// written by the original game.

namespace Rival {

static constexpr int numPlayers = 8;
static constexpr int numTroops = 42;
static constexpr int numTroopsPerRace = 14;
static constexpr int numMonsters = 30;
static constexpr int numBuildingTypes = 36;
static constexpr int numBuildingsPerRace = 12;
static constexpr int numBuildingsPlusCropland = numBuildingTypes + 1;
static constexpr int numProductionCosts = numBuildingsPlusCropland + numTroops;
static constexpr int numWeapons = 96;
static constexpr int numUpgrades = 112;
static constexpr int numAiStrategies = 7;

struct ScenarioHeader
{
    // 8 bytes: unknown
    bool wilderness;
    std::string mapName;
    std::uint32_t mapHeight;
    std::uint32_t mapWidth;
    // 9 bytes: unknown
};

struct PlayerProperties
{
    bool hasStartLocation;
    std::uint32_t startLocX;
    std::uint32_t startLocY;
    std::uint32_t startingFood;
    std::uint32_t startingWood;
    std::uint32_t startingGold;
    std::uint8_t race;
    bool ai;
    std::uint8_t aiType;
    std::uint8_t aiPerformance;
    std::uint8_t aiStrategy;
};

struct TroopDefaults
{  // also used for Monster Defaults
    std::uint16_t hitpoints;
    std::uint16_t magic;
    std::uint8_t armour;
    // 2 bytes: unknown
    std::uint8_t sight;
    std::uint8_t range;
    // 39 bytes: unknown
};

struct UpgradeProperties
{
    std::uint32_t amount;
    std::uint32_t goldCost;
    std::uint32_t woodCost;
    std::uint32_t unknown;
};

struct ProductionCost
{
    std::uint16_t goldCost;
    std::uint16_t woodCost;
    std::uint32_t constructionTime;
    std::uint32_t requiredExpOrIncreasePercent;
};

struct WeaponDefaults
{
    std::uint16_t moveSpaces;
    std::uint16_t moveTime;
    std::uint16_t damage;
    std::uint16_t penetrate;
    std::uint16_t accuracy;
    std::uint8_t effectRange;
    std::uint8_t attackRange;
    std::uint16_t manaCost;
    std::uint32_t reloadTime;
    std::uint16_t unknown;
    // 1 byte: empty
};

struct AvailableBuildings
{
    bool cropLand;
    bool goldAmplifier;
    bool rangedTroopBuilding;
    bool siegeTroopBuilding;
    bool meleeTroopBuilding;
    bool flyingTroopBuilding;
    bool engineerTroopBuilding;
    bool healerTroopBuilding;
    bool spellcasterTroopBuilding;
    bool shipyard;
    bool watchTower;
    bool wall;
};

struct HireTroopsRestrictions
{
    bool worker;
    bool rangedTroop;
    bool lightMeleeOrSpellcasterTroop;
    bool heavyMeleeTroop;
    bool engineer;
    bool stealthTroop;
    bool siegeTroop;
    bool raceBonusTroop;
    bool spellcaster;
    bool healer;
    bool transportShip;
    bool combatShip;
    bool flyingTroop;
    bool flyingTransport;
    bool mustHire;
};

struct AiSetting
{
    std::uint8_t amount;
    std::uint8_t flag;
};

struct AiStrategy
{
    std::array<AiSetting, numBuildingsPerRace> aiBuildingSettings;
    std::array<AiSetting, numTroopsPerRace> aiTroopSettings;
};

struct TilePlacement
{
    std::uint8_t type;
    std::uint8_t resource;
    std::uint8_t variant;
};

struct BuildingPlacement
{
    std::uint8_t type;
    // 1 byte: unknown (0x00 - 0x0f for Palisade)
    //     Always 0 in buildings example
    std::uint8_t wallVariant;
    // 1 byte: unknown (0x01 for Green Door, 0x02 for Blue Door, 0x03 for Yellow Door, 0x04 for Buildings)
    //     Always 0 in buildings example
    // 1 byte: unknown (0x08 for Palisade / Grate / Door)
    //     This is 4 for non-wall buildings, 0 for walls (building example)
    std::uint16_t x;
    std::uint16_t y;
    std::uint8_t player;
    std::uint16_t hitpoints;
    std::uint16_t armour;
    // 1 byte: empty
    //     Always 0 in buildings example
    std::uint8_t sight;
    std::uint8_t range;
    bool upgrade1Enabled;
    bool upgrade2Enabled;
    std::uint8_t specialColor;
    bool prisoner;
    std::string name;
    // 1 byte: empty
    //     Always 0 in buildings example
};

struct UnitPlacement
{
    std::uint8_t type;
    // 2 bytes: empty
    std::uint8_t facing;
    // 1 byte: empty
    std::uint16_t x;
    std::uint16_t y;
    std::uint8_t player;
    std::uint16_t hitpoints;
    std::uint8_t magic;
    std::uint16_t armour;
    // 1 bytes: empty
    std::uint8_t type2;
    std::uint8_t sight;
    std::uint8_t range;
    // 2 bytes: empty
    std::uint8_t specialColor;
    bool prisoner;
    std::uint16_t goldCost;
    std::uint16_t woodCost;
    std::string name;
    // 13 bytes: empty
    bool upgrade1Enabled;
    bool upgrade2Enabled;
    bool upgrade3Enabled;
    bool upgrade4Enabled;
    std::uint8_t fightingArea;
};

struct ChestPlacement
{
    std::uint32_t type;
    std::uint32_t variant;
    std::uint8_t x;
    std::uint8_t y;
};

struct InfoPointPlacement
{
    std::uint8_t x;
    std::uint8_t y;
    std::string message;
};

struct TrapPlacement
{
    std::uint8_t x;
    std::uint8_t y;
    std::uint8_t player;
};

struct ObjectPlacement
{
    std::uint8_t type;
    // 1 byte: empty
    std::uint8_t variant;
    std::uint32_t x;
    std::uint32_t y;
};

struct GoalLocation
{
    std::uint8_t type;
    std::uint8_t x;
    // 3 bytes: empty
    std::uint8_t y;
    // 3 bytes: empty
};

struct Goal
{
    std::uint8_t type;
    std::uint8_t x;
    std::uint8_t y;
    std::uint8_t count;
    std::uint8_t player;
    std::uint8_t radius;
    std::uint8_t troopId;
    std::uint8_t troopOrBuildingType;
    std::uint8_t itemType;
};

struct CampaignText
{
    std::string title;
    std::string objectives;
    std::string narration;
};

struct ScenarioData
{
    ScenarioHeader hdr {};
    std::array<PlayerProperties, numPlayers> playerProperties {};
    std::array<TroopDefaults, numTroops> troopDefaults {};
    std::array<TroopDefaults, numMonsters> monsterDefaults {};
    std::array<UpgradeProperties, numUpgrades> upgradeProperties {};
    std::array<ProductionCost, numProductionCosts> productionCosts {};
    std::array<WeaponDefaults, numWeapons> weaponDefaults {};
    AvailableBuildings availableBuildings {};
    HireTroopsRestrictions hireTroopsRestrictions {};
    std::array<AiStrategy, numAiStrategies> aiStrategies {};
    std::vector<TilePlacement> tiles {};
    std::vector<ObjectPlacement> objects {};
    std::vector<BuildingPlacement> buildings {};
    std::vector<UnitPlacement> units {};
    std::vector<ChestPlacement> chests {};
    std::vector<InfoPointPlacement> infoPoints {};
    std::vector<TrapPlacement> traps {};
    std::vector<GoalLocation> goalLocations {};
    std::vector<Goal> goals {};
    CampaignText campaignText {};
    std::uint8_t checksum {};

    ScenarioData()
    {
        // Initialize AI strategies
        for (int i = 0; i < numAiStrategies; i++)
        {
            aiStrategies[i] = AiStrategy();
        }
    }
};

}  // namespace Rival
