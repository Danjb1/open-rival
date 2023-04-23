#pragma once

#include <cstddef>  // std::size_t
#include <cstring>  // std::memcpy
#include <stdexcept>
#include <vector>

namespace BufferUtils {

/** Adds a value to the end of the given buffer. */
template <typename T>
void addToBuffer(std::vector<char>& buffer, const T& val)
{
    size_t requiredBufferSize = buffer.size() + sizeof(val);
    if (requiredBufferSize > buffer.capacity())
    {
        throw std::runtime_error("Trying to overfill buffer");
    }

    char* destPtr = buffer.data() + buffer.size();

    // Since we are writing to the vector's internal memory we need to manually resize the vector first
    buffer.resize(requiredBufferSize);

    // Later, we may need to ensure a certain endianness for cross-platform compatibility.
    // See: https://stackoverflow.com/questions/544928/reading-integer-size-bytes-from-a-char-array
    std::memcpy(destPtr, &val, sizeof(val));
}

/** Reads a value from the given buffer, at some offset.
 * The value is stored in dest, and the offset is increased by the size of the value. */
template <typename T>
void readFromBuffer(const std::vector<char>& buffer, std::size_t& offset, T& dest)
{
    if (offset + sizeof(dest) > buffer.size())
    {
        throw std::runtime_error("Trying to read past end of buffer");
    }

    std::memcpy(&dest, buffer.data() + offset, sizeof(dest));
    offset += sizeof(dest);
}

}  // namespace BufferUtils
