#include "entity/components/HealthComponent.h"

#include "entity/Entity.h"
#include "utils/MathUtils.h"

namespace Rival {

const std::string HealthComponent::key = "health";

HealthComponent::HealthComponent(int maxHealth)
    : EntityComponent(key)
    , maxHealth(maxHealth)
    , currentHealth(maxHealth)
{
}

void HealthComponent::addListener(std::weak_ptr<HealthListener> listener)
{
    if (listener.expired())
    {
        return;
    }
    listeners.emplace(listener);
}

void HealthComponent::removeListener(std::weak_ptr<HealthListener> listener)
{
    if (listener.expired())
    {
        return;
    }
    listeners.erase(listener);
}

void HealthComponent::addHealth(int amount)
{
    int prevHealth = currentHealth;
    currentHealth = MathUtils::clampi(currentHealth + amount, 0, maxHealth);

    CollectionUtils::forEachWeakPtr<HealthListener>(
            listeners, [&](auto listener) { listener->onHealthChanged(prevHealth, currentHealth); });

    if (currentHealth == 0)
    {
        CollectionUtils::forEachWeakPtr<HealthListener>(
                listeners, [&](auto listener) { listener->onHealthDepleted(); });
    }
}

}  // namespace Rival
