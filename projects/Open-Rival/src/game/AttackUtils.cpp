#include "game/AttackUtils.h"

#include <algorithm>  // std::max

#include "entity/Entity.h"
#include "entity/components/ArmorComponent.h"
#include "entity/components/HealthComponent.h"
#include "game/AttackDef.h"
#include "utils/MathUtils.h"

namespace Rival { namespace AttackUtils {

void applyAttack(const AttackDef& attackDef, Entity& target, std::mt19937& randomizer)
{
    HealthComponent* healthComp = target.getComponent<HealthComponent>();
    if (!healthComp)
    {
        // Entity cannot be harmed
        return;
    }

    // Damage dealt is random based on the attack accuracy
    const float accuracyRatio = MathUtils::clampf(attackDef.accuracy / 100.f, 0.f, 100.f);
    const int minDamage = static_cast<int>(attackDef.damage * accuracyRatio);
    std::uniform_int_distribution<int> distribution(minDamage, attackDef.damage);
    int damage = distribution(randomizer);

    // Reduce damage by the target's armor
    if (ArmorComponent* armorComp = target.getComponent<ArmorComponent>())
    {
        const int armor = armorComp->getArmor();
        damage = std::max(damage - armor, 0);
    }

    // Damage target
    healthComp->addHealth(-damage);
}

}}  // namespace Rival::AttackUtils
