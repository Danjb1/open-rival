#ifndef SCENARIO_READER_H
#define SCENARIO_READER_H

#include <cstdint>
#include <string>
#include <vector>

#include "Unit.h"

namespace Rival {

    struct ScenarioHeader {
        uint32_t unknown1;
        uint32_t unknown2;
        bool wilderness;
        std::string mapName;
        uint32_t mapHeight;
        uint32_t mapWidth;
        // 9 bytes: unknown
    };

    struct PlayerProperties {
        // 12 bytes: unknown
        uint32_t startingFood;
        uint32_t startingWood;
        uint32_t startingGold;
        uint8_t race;
        bool ai;
        uint8_t aiType;
        uint8_t aiPerformance;
        uint8_t aiStrategy;
    };

    class ScenarioReader {

    public:

        ScenarioReader(const std::string filename);

    private:

        const int numBytesInt = 4;

        size_t pos = 0;

        ScenarioHeader parseHeader(std::vector<unsigned char>& data);

        PlayerProperties parsePlayerProperties(
                std::vector<unsigned char>& data);

        std::uint8_t readByte(std::vector<unsigned char>& data);

        std::uint8_t readByte(
                std::vector<unsigned char>& data, int offset) const;

        bool readBool(std::vector<unsigned char>& data);

        bool readBool(
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

        void skip(const size_t n);

        void printNext(std::vector<unsigned char>& data, int n) const;

        void print(ScenarioHeader& hdr) const;

        void print(PlayerProperties& props) const;

    };

}

#endif // SCENARIO_READER_H
