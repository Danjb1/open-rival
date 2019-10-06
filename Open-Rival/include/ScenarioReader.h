#ifndef SCENARIO_READER_H
#define SCENARIO_READER_H

#include <cstdint>
#include <string>
#include <vector>

#include "Scenario.h"
#include "ScenarioData.h"
#include "Unit.h"

namespace Rival {

    class ScenarioReader {

    public:

        ScenarioReader(const std::string filename);

        ScenarioData readScenario();

        ScenarioData readCampaignScenario(int levelIndex);

    private:

        static const std::map<std::uint8_t, char> alphabet;

        static const int numBytesShort = 2;
        static const int numBytesInt = 4;

        static const int bytesPerTile = 6;

        std::vector<unsigned char> data;
        size_t pos = 0;

        void ScenarioReader::readFileContents(const std::string filename);

        ///////////////////////////////////////////////////////////////////////
        // Parsing
        ///////////////////////////////////////////////////////////////////////

        ScenarioData parseScenario(bool expectEof);

        ScenarioHeader parseHeader();

        PlayerProperties parsePlayerProperties();

        TroopDefaults parseTroopDefaults();

        bool ScenarioReader::doesUpgradeHaveAmount(int i) const;

        UpgradeProperties parseUpgradeProperties(bool readAmount);

        ProductionCost parseProductionCost();

        WeaponDefaults parseWeaponDefaults();

        AvailableBuildings parseAvailableBuildings();

        HireTroopsRestrictions parseHireTroopsRestrictions();

        AiSetting parseAiSetting();

        std::vector<TilePlacement> ScenarioReader::parseTiles(
                int width, int height);

        TilePlacement ScenarioReader::parseTile();

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

        std::uint8_t readByte(size_t offset) const;

        std::uint8_t readRivalByte();

        std::uint8_t readRivalByte(size_t offset) const;

        std::uint8_t fixRivalByte(std::uint8_t val) const;

        std::uint16_t readRivalShort();

        std::uint16_t readRivalShort(size_t offset) const;

        bool readBool();

        bool readBool(size_t offset) const;

        std::uint16_t readShort();

        std::uint16_t readShort(size_t offset) const;

        std::uint32_t readInt();

        std::uint32_t readInt(size_t offset) const;

        std::string readString(size_t length);

        std::string readString(size_t offset, size_t length) const;

        std::string readRivalString(size_t length);

        std::string readRivalString(size_t offset, size_t length) const;

        char getRivalChar(std::uint8_t c) const;

        void skip(const size_t n, bool print);

        ///////////////////////////////////////////////////////////////////////
        // Printing
        ///////////////////////////////////////////////////////////////////////

        void ScenarioReader::printOffset() const;

        void printSection(std::string title) const;

        void printNext(const size_t n) const;

        void print(ScenarioHeader& hdr) const;

        void print(PlayerProperties& props) const;

        void print(TroopDefaults& troop) const;

        void print(UpgradeProperties& upgrade) const;

        void print(ProductionCost& unitCost) const;

        void print(WeaponDefaults& wpn) const;

        void print(AvailableBuildings& bldg) const;

        void print(HireTroopsRestrictions& restrictions) const;

        void print(AiSetting& setting) const;

        void print(ObjectPlacement& obj) const;

        void print(BuildingPlacement& bldg) const;

        void print(UnitPlacement& unit) const;

        void print(TrapPlacement& trap) const;

        void print(GoalLocation& trap) const;

        void print(CampaignText& text) const;

    };

}

#endif // SCENARIO_READER_H
