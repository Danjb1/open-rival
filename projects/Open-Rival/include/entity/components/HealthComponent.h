#pragma once

#include <string>

#include "entity/components/EntityComponent.h"
#include "utils/CollectionUtils.h"

namespace Rival {

/**
 * Interface used to listen for health events.
 */
class HealthListener
{
public:
    virtual void onHealthChanged(int prevValue, int newValue) = 0;
    virtual void onMaxHealthChanged(int prevValue, int newValue) = 0;
    virtual void onHealthDepleted() = 0;
};

/**
 * Component that manages an Entity's facing.
 */
class HealthComponent : public EntityComponent
{

public:
    HealthComponent(int maxHealth);

    void addListener(std::weak_ptr<HealthListener> listener);
    void removeListener(std::weak_ptr<HealthListener> listener);

    void addHealth(int amount);

    int getHealth() const
    {
        return currentHealth;
    }

    int getMaxHealth() const
    {
        return maxHealth;
    }

    float getHealthRatio() const
    {
        return static_cast<float>(currentHealth) / maxHealth;
    }

public:
    static const std::string key;

private:
    WeakPtrSet<HealthListener> listeners;

    int maxHealth;
    int currentHealth;
};

}  // namespace Rival
