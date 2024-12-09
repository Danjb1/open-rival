#include "entity/Projectile.h"

namespace Rival {

Projectile::Projectile(AttackDef attackDef)
    : Entity(staticEntityType, /* isBig = */ false)
    , attackDef(attackDef)
{
}

}  // namespace Rival
