#include "game/AttackUtils.h"

#include "entity/Entity.h"
#include "entity/components/HealthComponent.h"
#include "game/AttackDef.h"
#include "utils/MathUtils.h"

namespace Rival { namespace AttackUtils {

void applyAttack(const AttackDef& attackDef, Entity& target, std::mt19937& randomizer)
{
    // Damage dealt is random based on the attack accuracy
    const float accuracyRatio = MathUtils::clampf(attackDef.accuracy / 100.f, 0.f, 100.f);
    const int minDamage = static_cast<int>(attackDef.damage * accuracyRatio);
    std::uniform_int_distribution<int> distribution(minDamage, attackDef.damage);
    const int damage = distribution(randomizer);

    // TODO: Reduce damage by the target's armor

    // Damage target
    HealthComponent* healthComp = target.getComponent<HealthComponent>();
    healthComp->addHealth(-damage);
}

}}  // namespace Rival::AttackUtils
