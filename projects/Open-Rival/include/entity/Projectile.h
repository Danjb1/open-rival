#pragma once

#include "entity/Entity.h"
#include "game/AttackDef.h"
#include "game/MapUtils.h"

namespace Rival {

/** A projectile that can be added to the world. */
class Projectile : public Entity
{
public:
    static constexpr EntityType staticEntityType = EntityType::Projectile;

public:
    Projectile(AttackDef attackDef);

    const AttackDef* getAttackDef() const
    {
        return &attackDef;
    }

private:
    AttackDef attackDef;
};

}  // namespace Rival
