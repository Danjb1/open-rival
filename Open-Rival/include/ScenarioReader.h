#ifndef SCENARIO_READER_H
#define SCENARIO_READER_H

#include <cstdint>
#include <string>
#include <vector>

#include "Unit.h"

namespace Rival {

    struct ScenarioHeader {
        std::uint32_t unknown1;
        std::uint32_t unknown2;
        bool wilderness;
        std::string mapName;
        std::uint32_t mapHeight;
        std::uint32_t mapWidth;
        // 9 bytes: unknown
    };

    struct PlayerProperties {
        // 12 bytes: unknown
        std::uint32_t startingFood;
        std::uint32_t startingWood;
        std::uint32_t startingGold;
        std::uint8_t race;
        bool ai;
        std::uint8_t aiType;
        std::uint8_t aiPerformance;
        std::uint8_t aiStrategy;
    };

    struct TroopDefaults {  // also used for Monster Defaults
        std::uint16_t hitpoints;
        std::uint16_t magic;
        std::uint8_t armour;
        // 2 bytes: unknown
        std::uint8_t sight;
        std::uint8_t range;
        // 39 bytes: unknown
    };

    struct UpgradeProperties {
        std::uint32_t amount;
        std::uint32_t goldCost;
        std::uint32_t woodCost;
        std::uint32_t unknown;
    };

    struct UnitProductionCost {
        std::uint16_t goldCost;
        std::uint16_t woodCost;
        std::uint32_t constructionTime;
        std::uint32_t requiredExpOrIncreasePercent;
    };

    struct WeaponDefaults {
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

    struct AvailableBuildings {
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

    struct HireTroopsRestrictions {
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

    struct TilePlacement {
        std::uint8_t length;
    };

    struct BuildingPlacement {
        std::uint8_t type;
        // 2 bytes: unknown
        std::uint8_t player;
        std::uint16_t x;
        std::uint16_t y;
        std::uint16_t hitpoints;
        std::uint16_t armour;
        // 1 byte: empty
        std::uint8_t sight;
        std::uint8_t range;
        bool upgrade1Enabled;
        bool upgrade2Enabled;
        std::uint8_t specialColour;
        bool prisoner;
        std::string name;
    };

    struct UnitPlacement {
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
        std::uint8_t specialColour;
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

    struct TrapPlacement {
        std::uint8_t x;
        std::uint8_t y;
        std::uint8_t player;
    };

    class ScenarioReader {

    public:

        ScenarioReader(const std::string filename);

    private:

        const int numBytesShort = 2;
        const int numBytesInt = 4;

        const int numTroops = 42;
        const int numMonsters = 30;
        const int numBuildings = 36;
        const int numBuildingsPlusCropland = numBuildings + 1;
        const int numWeapons = 96;
        const int numUpgrades = 112;

        size_t pos = 0;

        ///////////////////////////////////////////////////////////////////////////
        // Parsing
        ///////////////////////////////////////////////////////////////////////////

        PlayerProperties parsePlayerProperties(
                std::vector<unsigned char>& data);

        TroopDefaults parseTroopDefaults(std::vector<unsigned char>& data);

        bool ScenarioReader::doesUpgradeHaveAmount(int i) const;

        UpgradeProperties parseUpgradeProperties(
                std::vector<unsigned char>& data, bool readAmount);

        UnitProductionCost parseUnitProductionCost(
                std::vector<unsigned char>& data);

        WeaponDefaults parseWeaponDefaults(std::vector<unsigned char>& data);

        AvailableBuildings parseAvailableBuildings(
                std::vector<unsigned char>& data);

        HireTroopsRestrictions parseHireTroopsRestrictions(
                std::vector<unsigned char>& data);

        TilePlacement parseTile(std::vector<unsigned char>& data);

        BuildingPlacement parseBuilding(std::vector<unsigned char>& data);

        UnitPlacement parseUnit(std::vector<unsigned char>& data);

        TrapPlacement parseTrap(std::vector<unsigned char>& data);

        ///////////////////////////////////////////////////////////////////////////
        // Token Extraction
        ///////////////////////////////////////////////////////////////////////////

        std::uint8_t readByte(std::vector<unsigned char>& data);

        std::uint8_t readByte(
                std::vector<unsigned char>& data, int offset) const;

        bool readBool(std::vector<unsigned char>& data);

        bool readBool(
                std::vector<unsigned char>& data, int offset) const;

        std::uint16_t readShort(std::vector<unsigned char>& data);

        std::uint16_t readShort(
            std::vector<unsigned char>& data, int offset) const;

        std::uint32_t readInt(std::vector<unsigned char>& data);

        std::uint32_t readInt(
                std::vector<unsigned char>& data, int offset) const;

        std::string readString(
                std::vector<unsigned char>& data, size_t length);

        std::string readString(
                std::vector<unsigned char>& data,
                int offset,
                size_t length) const;

        void skip(std::vector<unsigned char>& data, const size_t n);

        ///////////////////////////////////////////////////////////////////////////
        // Printing
        ///////////////////////////////////////////////////////////////////////////

        void ScenarioReader::printOffset() const;

        ScenarioHeader parseHeader(std::vector<unsigned char>& data);

        void printSection(std::string title) const;

        void printNext(std::vector<unsigned char>& data, const size_t n) const;

        void print(ScenarioHeader& hdr) const;

        void print(PlayerProperties& props) const;

        void print(TroopDefaults& troop) const;

        void print(UpgradeProperties& upgrade) const;

        void print(UnitProductionCost& unitCost) const;

        void print(WeaponDefaults& wpn) const;

        void print(AvailableBuildings& bldg) const;

        void print(HireTroopsRestrictions& restrictions) const;

        void print(BuildingPlacement& bldg) const;

        void print(UnitPlacement& unit) const;

        void print(TrapPlacement& trap) const;

    };

}

#endif // SCENARIO_READER_H
