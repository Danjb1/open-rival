#pragma once

#include <random>

namespace Rival {

class AttackDef;
class Entity;

namespace AttackUtils {

void applyAttack(const AttackDef& attackDef, Entity& target, std::mt19937& randomizer);

}
}  // namespace Rival
