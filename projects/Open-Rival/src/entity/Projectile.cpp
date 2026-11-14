#include "entity/Projectile.h"

#include "gfx/RenderUtils.h"

namespace Rival {

Projectile::Projectile(AttackDef attackDef)
    : Entity(staticEntityType, /* isBig = */ false)
    , attackDef(attackDef)
{
}

std::string Projectile::getDebugName() const
{
    return "[Projectile] " + attackDef.name + " (" + std::to_string(id) + ")";
}

float Projectile::getZOffset() const
{
    // Projectiles should be rendered on top of other entities!
    return RenderUtils::zOffsetProjectiles;
}

}  // namespace Rival
