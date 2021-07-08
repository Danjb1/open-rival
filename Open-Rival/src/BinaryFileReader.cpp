#include "pch.h"
#include "BinaryFileReader.h"

#include <stdexcept>

namespace Rival {

    BinaryFileReader::BinaryFileReader(const std::string filename)
        : in(std::ifstream(filename, std::ios::binary | std::ios::in)) {
        if (!in) {
            throw std::runtime_error("Failed to load image!");
        }
    }

    void BinaryFileReader::skip(int n) {
        in.ignore(n);
    }

    std::uint8_t BinaryFileReader::readByte() {
        return static_cast<std::uint8_t>(in.get());
    }

    std::uint16_t BinaryFileReader::readShort() {
        std::uint16_t val;
        in.read(reinterpret_cast<char*>(&val), sizeof(val));
        return val;
    }

    void BinaryFileReader::read(std::vector<unsigned char>* buffer) {
        in.read(reinterpret_cast<char*>(buffer->data()), buffer->size());
    }

}  // namespace Rival
