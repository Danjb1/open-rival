#pragma once

#include <cstdint>

namespace Rival {

enum class EntityType : std::uint8_t
{
    Unknown,
    Unit,
    Building,
    Projectile,
    Tree,
    Mountain,
    Wall,
    Palisade,
    Door,
    Decoration,
    Chest,
    InfoPoint,
    Effect
};

const auto firstEntityType { static_cast<int>(EntityType::Unknown) };
const auto lastEntityType { static_cast<int>(EntityType::InfoPoint) };

}  // namespace Rival
