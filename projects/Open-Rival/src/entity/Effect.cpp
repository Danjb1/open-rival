#include "entity/Effect.h"

#include "gfx/RenderUtils.h"

namespace Rival {

void Effect::addListener(std::weak_ptr<EffectListener> listener)
{
    if (listener.expired())
    {
        return;
    }
    listeners.emplace(listener);
}

void Effect::removeListener(std::weak_ptr<EffectListener> listener)
{
    if (listener.expired())
    {
        return;
    }
    listeners.erase(listener);
}

void Effect::onAnimationFinished()
{
    CollectionUtils::forEachWeakPtr<EffectListener>(
            listeners, [&](auto listener) { listener->onEffectFinished(this); });
    markForDeletion();
}

float Effect::getZOffset() const
{
    // Effects should be rendered on top of other entities!
    return RenderUtils::zOffsetEffects;
}

}  // namespace Rival
