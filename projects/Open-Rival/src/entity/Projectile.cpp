#include "entity/Projectile.h"

namespace Rival {

Projectile::Projectile(const AttackDef& attackDef)
    : Entity(staticEntityType, /* isBig = */ false)
    , attackDef(attackDef)
{
}

}  // namespace Rival
