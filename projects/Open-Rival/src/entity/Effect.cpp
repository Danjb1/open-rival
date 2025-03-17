#include "entity/Effect.h"

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

}  // namespace Rival
