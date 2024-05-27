#pragma once

#include <cstddef>  // std::size_t
#include <cstring>  // std::memcpy
#include <stdexcept>
#include <string>
#include <vector>

namespace Rival { namespace BufferUtils {

/** Adds a value to the end of the given buffer.
 * This should not be used for anything which manages its own memory (e.g. containers and strings). */
template <typename T>
void addToBuffer(std::vector<char>& buffer, const T& val)
{
    // Later, we may need to ensure a certain endianness for cross-platform compatibility.
    // See: https://stackoverflow.com/questions/544928/reading-integer-size-bytes-from-a-char-array

    std::size_t requiredBufferSize = buffer.size() + sizeof(val);
    if (requiredBufferSize > buffer.capacity())
    {
        throw std::runtime_error("Trying to overfill buffer");
    }

    char* destPtr = buffer.data() + buffer.size();

    // Since we are writing to the vector's internal memory we need to manually change the size
    buffer.resize(requiredBufferSize);

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

/** Adds a vector to the end of the given buffer. */
template <typename T>
void addVectorToBuffer(std::vector<char>& buffer, const std::vector<T>& vec)
{
    std::size_t numElems = vec.size();
    std::size_t dataSize = sizeof(T) * numElems;
    std::size_t requiredBufferSize = buffer.size() + sizeof(numElems) + dataSize;
    if (requiredBufferSize > buffer.capacity())
    {
        throw std::runtime_error("Trying to overfill buffer");
    }

    char* destPtr = buffer.data() + buffer.size();

    // Since we are writing to the vector's internal memory we need to manually change the size
    buffer.resize(requiredBufferSize);

    // Write the number of elements
    std::memcpy(destPtr, &numElems, sizeof(numElems));
    destPtr += sizeof(numElems);

    // Write the data
    std::memcpy(destPtr, &vec[0], dataSize);
}

/** Reads a vector from the given buffer, at some offset.
 * The offset is increased by the size of the data read. */
template <typename T>
std::vector<T> readVectorFromBuffer(const std::vector<char>& buffer, std::size_t& offset)
{
    std::size_t numElems = 0;
    readFromBuffer(buffer, offset, numElems);

    if (offset + numElems * sizeof(T) > buffer.size())
    {
        throw std::runtime_error("Trying to read past end of buffer");
    }

    std::vector<T> outVector;
    outVector.reserve(numElems);

    for (std::size_t i = 0; i < numElems; ++i)
    {
        T elem;
        std::memcpy(&elem, buffer.data() + offset, sizeof(elem));
        offset += sizeof(elem);
        outVector.push_back(elem);
    }

    return outVector;
}

/** Adds a string to the end of the given buffer. */
void addStringToBuffer(std::vector<char>& buffer, const std::string& s);

/** Reads a string from the given buffer, at some offset.
 * The offset is increased by the size of the data read. */
std::string readStringFromBuffer(const std::vector<char>& buffer, std::size_t& offset);

}}  // namespace Rival::BufferUtils
