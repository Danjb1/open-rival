#ifndef CONFIG_UTILS_H
#define CONFIG_UTILS_H

#include "json.h"

using json = nlohmann::json;

namespace Rival { namespace ConfigUtils {

    template <class T>
    T get(json cfg, std::string key, T defaultValue)
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

#endif  // CONFIG_UTILS_H
