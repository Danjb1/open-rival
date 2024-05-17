#pragma once

#include <cstddef>  // std::size_t
#include <cstdint>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "game/UnitType.h"
#include "scenario/ScenarioData.h"

namespace Rival {

class ScenarioReader
{

public:
    ScenarioReader(const std::string& filename);

    ScenarioData readScenario();

    ScenarioData readCampaignScenario(int levelIndex);

private:
    ///////////////////////////////////////////////////////////////////////
    // Parsing
    ///////////////////////////////////////////////////////////////////////

    ScenarioData parseScenario(bool expectEof);

    ScenarioHeader parseHeader();

    PlayerProperties parsePlayerProperties();

    TroopDefaults parseTroopDefaults();

    bool doesUpgradeHaveAmount(int i) const;

    UpgradeProperties parseUpgradeProperties(bool readAmount);

    ProductionCost parseProductionCost();

    WeaponDefaults parseWeaponDefaults();

    AvailableBuildings parseAvailableBuildings();

    HireTroopsRestrictions parseHireTroopsRestrictions();

    AiSetting parseAiSetting();

    std::vector<TilePlacement> parseTiles(int width, int height);

    TilePlacement parseTile();

    std::vector<ObjectPlacement> parseObjects();

    ObjectPlacement parseObject();

    std::vector<BuildingPlacement> parseBuildings();

    BuildingPlacement parseBuilding();

    std::vector<UnitPlacement> parseUnits();

    UnitPlacement parseUnit();

    std::vector<ChestPlacement> parseChests();

    ChestPlacement parseChest();

    std::vector<InfoPointPlacement> parseInfoPoints();

    InfoPointPlacement parseInfoPoint();

    std::vector<TrapPlacement> parseTraps();

    TrapPlacement parseTrap();

    std::vector<GoalLocation> parseGoalLocations();

    GoalLocation parseGoalLocation();

    std::vector<Goal> parseGoals();

    Goal parseGoal();

    CampaignText parseCampaignText();

    ///////////////////////////////////////////////////////////////////////
    // Token Extraction
    ///////////////////////////////////////////////////////////////////////

    std::uint8_t readByte();

    std::uint8_t readByte(std::size_t offset) const;

    std::uint8_t readRivalByte();

    std::uint8_t readRivalByte(std::size_t offset) const;

    std::uint8_t fixRivalByte(std::uint8_t val) const;

    std::uint16_t readRivalShort();

    std::uint16_t readRivalShort(std::size_t offset) const;

    bool readBool();

    bool readBool(std::size_t offset) const;

    std::uint16_t readShort();

    std::uint16_t readShort(std::size_t offset) const;

    std::uint32_t readInt();

    std::uint32_t readInt(std::size_t offset) const;

    std::string readString(std::size_t length);

    std::string readString(std::size_t offset, std::size_t length) const;

    std::string readRivalString(std::size_t length);

    std::string readRivalString(std::size_t offset, std::size_t length) const;

    char getRivalChar(std::uint8_t c) const;

    void skip(const std::size_t n, bool print);

    ///////////////////////////////////////////////////////////////////////
    // Printing
    ///////////////////////////////////////////////////////////////////////

    void printOffset() const;

    void printSection(std::string title) const;

    void printNext(const std::size_t n) const;

    void print(const ScenarioHeader& hdr) const;

    void print(const PlayerProperties& props) const;

    void print(const TroopDefaults& troop) const;

    void print(const UpgradeProperties& upgrade) const;

    void print(const ProductionCost& unitCost) const;

    void print(const WeaponDefaults& wpn) const;

    void print(const AvailableBuildings& bldg) const;

    void print(const HireTroopsRestrictions& restrictions) const;

    void print(const AiSetting& setting) const;

    void print(const ObjectPlacement& obj) const;

    void print(const BuildingPlacement& bldg) const;

    void print(const UnitPlacement& unit) const;

    void print(const TrapPlacement& trap) const;

    void print(const GoalLocation& trap) const;

    void print(const CampaignText& text) const;

private:
    static const std::unordered_map<std::uint8_t, char> alphabet;

    static constexpr int numBytesShort = 2;
    static constexpr int numBytesInt = 4;

    static constexpr int bytesPerTile = 6;

    std::vector<unsigned char> data;
    std::size_t pos = 0;

    mutable std::stringstream log;
};

}  // namespace Rival
