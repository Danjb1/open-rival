#include "game/AttackDef.h"

namespace Rival {

AttackDef AttackDef::fromJson(const json& j)
{
    AttackDef attackDef;
    attackDef.name = j.at("name");
    attackDef.accuracy = j.value("accuracy", 0);
    attackDef.damage = j.value("damage", 0);
    attackDef.impactSound = j.value("impactSound", "");
    attackDef.manaCost = j.value("manaCost", 0);
    attackDef.projectile = j.value("projectile", "");
    attackDef.projectileSpeed = j.value("projectileSpeed", 0);
    attackDef.range = j.value("range", 1);
    attackDef.reloadTime = j.value("reloadTime", 0);
    attackDef.sound = j.value("sound", "");
    attackDef.hasSplashDamage = j.value("splashDamage", false);

    return attackDef;
}

}  // namespace Rival
