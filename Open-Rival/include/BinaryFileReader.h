#ifndef BINARY_FILE_READER_H
#define BINARY_FILE_READER_H

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace Rival {

    class BinaryFileReader {

    public:
        BinaryFileReader(const std::string filename);

        void skip(int n);

        std::streampos getPos();

        void setPos(std::streampos pos);

        std::uint8_t readByte();

        std::uint16_t readShort();

        void read(std::vector<unsigned char>* buffer);

    private:
        std::ifstream in;
    };

}  // namespace Rival

#endif  // BINARY_FILE_READER_H
