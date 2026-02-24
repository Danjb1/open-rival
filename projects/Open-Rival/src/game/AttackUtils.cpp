#include "game/AttackUtils.h"

#include <algorithm>  // std::max
#include <cmath>      // std::floor

#include "entity/Entity.h"
#include "entity/components/ArmorComponent.h"
#include "entity/components/HealthComponent.h"
#include "game/AttackDef.h"
#include "utils/MathUtils.h"

namespace Rival { namespace AttackUtils {

bool tryApplyAttack(const AttackDef& attackDef, Entity& target, std::mt19937& randomizer, float damageMultiplier)
{
    HealthComponent* healthComp = target.getComponent<HealthComponent>();
    if (!healthComp)
    {
        // Only Entities with a HealthComponent can be harmed
        return false;
    }

    if (healthComp->isDead())
    {
        // Entity is already dead
        return false;
    }

    // Calculate the base damage.
    // Damage dealt is random based on the attack accuracy; with 50 Accuracy, 50-100 % of damage is dealt
    const float accuracyRatio = MathUtils::clampf(attackDef.accuracy / 100.f, 0.f, 100.f);
    const int minDamage = static_cast<int>(attackDef.damage * accuracyRatio);
    std::uniform_int_distribution<int> distribution(minDamage, attackDef.damage);
    int damage = distribution(randomizer);

    // Apply damage multiplier
    if (damageMultiplier != 1.f)
    {
        damage = static_cast<int>(std::floor(damage * damageMultiplier));
    }

    // Reduce damage by the target's armor
    if (ArmorComponent* armorComp = target.getComponent<ArmorComponent>())
    {
        const int armor = armorComp->getArmor();
        damage = std::max(damage - armor, 0);
    }

    // Damage target
    healthComp->addHealth(-damage);

    return true;
}

}}  // namespace Rival::AttackUtils
