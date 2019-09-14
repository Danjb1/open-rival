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

    struct TroopDefaults {
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

    class ScenarioReader {

    public:

        ScenarioReader(const std::string filename);

    private:

        const int numBytesShort = 2;
        const int numBytesInt = 4;

        const int numTroops = 42;
        const int numUpgrades = 112;

        size_t pos = 0;

        void ScenarioReader::printOffset() const;

        ScenarioHeader parseHeader(std::vector<unsigned char>& data);

        PlayerProperties parsePlayerProperties(
                std::vector<unsigned char>& data);

        TroopDefaults parseTroopDefaults(std::vector<unsigned char>& data);

        bool ScenarioReader::doesUpgradeHaveAmount(int i) const;

        UpgradeProperties parseUpgradeProperties(
                std::vector<unsigned char>& data, bool readAmount);

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

        void printSection(std::string title) const;

        void printNext(std::vector<unsigned char>& data, const size_t n) const;

        void print(ScenarioHeader& hdr) const;

        void print(PlayerProperties& props) const;

        void print(TroopDefaults& props) const;

        void print(UpgradeProperties& upgrade) const;

    };

}

#endif // SCENARIO_READER_H
