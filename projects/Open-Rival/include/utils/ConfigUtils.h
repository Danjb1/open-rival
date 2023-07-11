#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Rival { namespace ConfigUtils {

template <class T>
T get(const json& cfg, const std::string& key, const T& defaultValue)
{
    if (cfg.contains(key))
    {
        return cfg[key];
    }
    else
    {
        return defaultValue;
    }
}

}}  // namespace Rival::ConfigUtils
