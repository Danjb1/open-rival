#pragma once

namespace Rival {

class AttackDef;
class Entity;

namespace DamageUtils {

void applyAttack(const AttackDef& attackDef, std::shared_ptr<Entity> target);

}
}  // namespace Rival
