#include "utils/BufferUtils.h"

namespace Rival { namespace BufferUtils {

void addStringToBuffer(std::vector<char>& buffer, const std::string& s)
{
    std::size_t numChars = s.size();
    std::size_t requiredBufferSize = buffer.size() + sizeof(numChars) + sizeof(char) * numChars;
    if (requiredBufferSize > buffer.capacity())
    {
        throw std::runtime_error("Trying to overfill buffer");
    }

    char* destPtr = buffer.data() + buffer.size();

    // Since we are writing to the vector's internal memory we need to manually change the size
    buffer.resize(requiredBufferSize);

    // Write the number of characters
    std::memcpy(destPtr, &numChars, sizeof(numChars));
    destPtr += sizeof(numChars);

    // Write each character
    for (std::size_t i = 0; i < numChars; ++i)
    {
        std::memcpy(destPtr, &s.c_str()[i], sizeof(char));
        destPtr += sizeof(char);
    }
}

std::string readStringFromBuffer(const std::vector<char>& buffer, std::size_t& offset)
{
    std::size_t strLength = 0;
    readFromBuffer(buffer, offset, strLength);

    if (offset + strLength * sizeof(char) > buffer.size())
    {
        throw std::runtime_error("Trying to read past end of buffer");
    }

    std::string s;
    s.reserve(strLength);

    for (std::size_t i = 0; i < strLength; ++i)
    {
        char c;
        std::memcpy(&c, buffer.data() + offset, sizeof(c));
        offset += sizeof(c);
        s += c;
    }

    return s;
}

}}  // namespace Rival::BufferUtils