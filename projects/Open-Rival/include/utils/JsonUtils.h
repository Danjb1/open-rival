#pragma once

#pragma warning(push)
// NOTE: This warning is fixed in later versions of the library: https://github.com/nlohmann/json/issues/3805
#pragma warning(disable : 26800)
#include <nlohmann/json.hpp>
#pragma warning(pop)

#include <string>

using json = nlohmann::json;

namespace Rival { namespace JsonUtils {

template <class Iterator, class KeyType, class ValueType>
const ValueType getOrDefault(const Iterator& iter, const KeyType& key, const ValueType defaultValue)
{
    auto result = iter->find(key);
    return result == iter->end() ? defaultValue : static_cast<ValueType>(*result);
}

json readJsonFile(const std::string& filename);

}}  // namespace Rival::JsonUtils
