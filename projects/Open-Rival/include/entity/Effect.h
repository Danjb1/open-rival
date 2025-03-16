#pragma once

#include "entity/Entity.h"
#include "game/EffectDef.h"

namespace Rival {

/** A visual effect that can be added to the world. */
class Effect : public Entity
{
public:
    static constexpr EntityType staticEntityType = EntityType::Effect;

public:
    Effect(const EffectDef& effectDef)
        : Entity(staticEntityType, /* isBig = */ false)
        , effectDef(effectDef)
    {
    }

private:
    const EffectDef& effectDef;
};

}  // namespace Rival
