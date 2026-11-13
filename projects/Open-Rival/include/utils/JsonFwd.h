#pragma once

/*
 * Lightweight forward declaration of our json type.
 *
 * Prefer this over JsonUtils.h in header files that only refer to `json` by
 * reference, since the full nlohmann/json header is very slow to compile.
 */

#include <nlohmann/json_fwd.hpp>

using json = nlohmann::json;
