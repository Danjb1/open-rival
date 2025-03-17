#pragma once

#include <memory>

#include "entity/Entity.h"
#include "game/EffectDef.h"
#include "utils/CollectionUtils.h"

namespace Rival {

/** Interface used to listen to Effect. */
class EffectListener
{
public:
    virtual void onEffectFinished(class Effect* effect) = 0;
};

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

    void addListener(std::weak_ptr<EffectListener> listener);
    void removeListener(std::weak_ptr<EffectListener> listener);

    void onAnimationFinished();

private:
    const EffectDef& effectDef;

    WeakPtrSet<EffectListener> listeners;
};

}  // namespace Rival
