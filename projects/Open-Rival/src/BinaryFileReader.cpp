#include "BinaryFileReader.h"

#include <stdexcept>

namespace Rival {

BinaryFileReader::BinaryFileReader(const std::string filename)
    : in(std::ifstream(filename, std::ios::binary | std::ios::in))
{
    if (!in)
    {
        throw std::runtime_error("Failed to load file: " + filename);
    }
}

void BinaryFileReader::skip(int n)
{
    in.ignore(n);
}

std::streampos BinaryFileReader::getPos()
{
    return in.tellg();
}

void BinaryFileReader::setPos(std::streampos pos)
{
    in.seekg(pos);
}

std::uint8_t BinaryFileReader::readByte()
{
    return static_cast<std::uint8_t>(in.get());
}

std::uint16_t BinaryFileReader::readShort()
{
    std::uint16_t val = 0;
    in.read(reinterpret_cast<char*>(&val), sizeof(val));
    return val;
}

void BinaryFileReader::read(std::vector<std::uint8_t>* buffer)
{
    in.read(reinterpret_cast<char*>(buffer->data()), buffer->size());
}

std::streamoff BinaryFileReader::getBytesRemaining()
{
    std::streampos currentPos = getPos();
    in.seekg(0, std::ios::end);
    std::streampos endPos = getPos();
    setPos(currentPos);
    return endPos - currentPos;
}

}  // namespace Rival
