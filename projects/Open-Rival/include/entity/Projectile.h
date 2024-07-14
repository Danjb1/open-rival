#pragma once

#include "entity/Entity.h"

namespace Rival {

/** A projectile that can be added to the world. */
class Projectile : public Entity
{
public:
    static constexpr EntityType staticEntityType = EntityType::Projectile;

public:
    Projectile()
        : Entity(staticEntityType, /* isBig = */ false)
    {
    }
};

}  // namespace Rival
