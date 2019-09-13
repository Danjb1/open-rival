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

        // Parse file contents
        ScenarioHeader hdr = parseHeader(data);
        PlayerProperties props1 = parsePlayerProperties(data);
        PlayerProperties props2 = parsePlayerProperties(data);

        // Print map data
        print(hdr);
        print(props1);
        print(props2);
    }

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
        skip(9);

        return hdr;
    }

    PlayerProperties ScenarioReader::parsePlayerProperties(
            std::vector<unsigned char>& data) {

        PlayerProperties props;

        skip(12);
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

    std::uint8_t ScenarioReader::readByte(
            std::vector<unsigned char>& data) {
        std::uint8_t value = readByte(data, pos);
        pos += 1;
        return value;
    }

    std::uint8_t ScenarioReader::readByte(
            std::vector<unsigned char>& data, int offset) const {
        return std::uint8_t(data[offset + 0]);
    }

    bool ScenarioReader::readBool(std::vector<unsigned char>& data) {
        bool value = readBool(data, pos);
        pos += 1;
        return value;
    }

    bool ScenarioReader::readBool(
            std::vector<unsigned char>& data, int offset) const {
        uint8_t value = data[offset];
        return value == 1;
    }

    std::uint32_t ScenarioReader::readInt(
             std::vector<unsigned char>& data) {
        std::uint32_t value = readInt(data, pos);
        pos += numBytesInt;
        return value;
    }

    uint32_t ScenarioReader::readInt(
            std::vector<unsigned char>& data, int offset) const {
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
            int offset,
            size_t length) const {

        std::vector<char> nameChars(length);
        for (size_t i = 0; i < length; i++) {
            nameChars[i] = data[offset + i];
        }
        std::string value(nameChars.data(), length);
        return value;
    }

    void ScenarioReader::skip(const size_t n) {
        pos += n;
    }

    void ScenarioReader::printNext(
            std::vector<unsigned char>& data, int n) const {

        // Switch to hex
        std::cout << std::setfill('0') << std::hex;
        for (int i = 0; i < n; i++) {
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
            << "Starting Gold: " << props.startingGold << '\n'
            << "Starting Wood: " << props.startingWood << '\n'
            << "Starting Food: " << props.startingFood << '\n'
            << "Race: " << static_cast<int>(props.race) << '\n'
            << "AI: " << props.ai << '\n'
            << "AI Type: " << static_cast<int>(props.aiType) << '\n'
            << "AI Performance: " << static_cast<int>(props.aiPerformance) << '\n'
            << "AI Strategy: " << static_cast<int>(props.aiStrategy) << '\n';
    }

}
