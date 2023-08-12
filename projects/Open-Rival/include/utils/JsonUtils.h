#pragma once

#include <nlohmann/json.hpp>

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
