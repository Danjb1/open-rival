#pragma once

#include "entity/Entity.h"

namespace Rival {

/** A Decoration object that can be added to the world. */
class Decoration : public Entity
{
public:
    static constexpr EntityType staticEntityType = EntityType::Decoration;
    static constexpr int width = 1;
    static constexpr int height = 1;

public:
    Decoration()
        : Entity(staticEntityType, width, height)
    {
    }
};

}  // namespace Rival
