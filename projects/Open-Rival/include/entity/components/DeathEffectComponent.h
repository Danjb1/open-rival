#pragma once

#include <memory>
#include <string>

#include "entity/components/EntityComponent.h"
#include "game/EffectDef.h"
#include "game/MapUtils.h"

namespace Rival {

class Entity;
class EntityFactory;

/**
 * Component that can spawn a death effect.
 */
class DeathEffectComponent : public EntityComponent
{
public:
    DeathEffectComponent(const EffectDef& effectDef, std::shared_ptr<const EntityFactory> entityFactory);

    std::shared_ptr<Entity> spawnEffect(Facing facing = Facing::North);

public:
    static const std::string key;

private:
    const EffectDef& effectDef;
    std::shared_ptr<const EntityFactory> entityFactory;
};

}  // namespace Rival
