#pragma once

#include <random>

namespace Rival {

class AttackDef;
class Entity;

namespace AttackUtils {

bool tryApplyAttack(const AttackDef& attackDef, Entity& target, std::mt19937& randomizer, float damageMultiplier = 1.f);

}
}  // namespace Rival
