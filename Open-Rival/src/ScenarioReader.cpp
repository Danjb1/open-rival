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

        // Parse map header
        printSection("Parsing header");
        printOffset();
        ScenarioHeader hdr = parseHeader(data);
        print(hdr);

        // Parse player properties
        printSection("Parsing player properties");
        printOffset();
        PlayerProperties props1 = parsePlayerProperties(data);
        print(props1);
        PlayerProperties props2 = parsePlayerProperties(data);
        PlayerProperties props3 = parsePlayerProperties(data);
        PlayerProperties props4 = parsePlayerProperties(data);
        PlayerProperties props5 = parsePlayerProperties(data);
        PlayerProperties props6 = parsePlayerProperties(data);
        PlayerProperties props7 = parsePlayerProperties(data);
        PlayerProperties props8 = parsePlayerProperties(data);

        // Unknown
        printSection("Skipping unknown section");
        printOffset();
        skip(data, 909);

        // Parse troop defaults
        printSection("Parsing troop defaults");
        printOffset();
        TroopDefaults troop1 = parseTroopDefaults(data);
        print(troop1);
        TroopDefaults troop2 = parseTroopDefaults(data);
    }

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
        skip(data, 9);

        return hdr;
    }

    PlayerProperties ScenarioReader::parsePlayerProperties(
            std::vector<unsigned char>& data) {

        PlayerProperties props;

        skip(data, 12);
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
        skip(data, 2);
        troop.sight = readByte(data);
        troop.range = readByte(data);
        skip(data, 39);

        return troop;
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

    std::uint16_t ScenarioReader::readShort(
        std::vector<unsigned char>& data) {
        std::uint16_t value = readShort(data, pos);
        pos += numBytesShort;
        return value;
    }

    uint16_t ScenarioReader::readShort(
        std::vector<unsigned char>& data, int offset) const {
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

    void ScenarioReader::skip(
            std::vector<unsigned char>& data, const size_t n) {
        std::cout << "SKIP: ";
        printNext(data, n);
        pos += n;
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
        for (size_t i = 0; i < n; i++) {
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

}
