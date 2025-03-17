#include "entity/components/EffectAnimationComponent.h"

#include "entity/Effect.h"
#include "entity/Entity.h"
#include "game/EffectDef.h"

namespace Rival {

const std::string EffectAnimationComponent::key = "effectAnimation";

EffectAnimationComponent::EffectAnimationComponent(const EffectDef& effectDef)
    : BasicAnimationComponent(key)
    , effectDef(effectDef)
{
}

void EffectAnimationComponent::onEntityFirstAddedToWorld(World* world)
{
    BasicAnimationComponent::onEntityFirstAddedToWorld(world);

    // Animate immediately
    setAnimationData(effectDef.getAnimation());
}

void EffectAnimationComponent::animationFinished()
{
    BasicAnimationComponent::animationFinished();

    // Directly notify the Effect
    if (Effect* effect = entity->as<Effect>())
    {
        effect->onAnimationFinished();
    }
}

int EffectAnimationComponent::getFacingIndex(std::shared_ptr<FacingComponent> facingComponent) const
{
    // TODO: We might need to update this for some effects in the future
    return facingComponent->getDeathFacingIndex();
}

}  // namespace Rival
