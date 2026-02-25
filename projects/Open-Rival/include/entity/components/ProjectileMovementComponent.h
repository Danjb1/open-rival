#pragma once

#include <glm/vec2.hpp>

#include <memory>
#include <random>
#include <string>

#include "entity/components/EntityComponent.h"
#include "game/MapUtils.h"
#include "utils/EntityUtils.h"

namespace Rival {

class AttackDef;
class AudioStore;
class AudioSystem;
class EffectDef;
class EntityFactory;

/**
 * Component that moves a projectile to its destination.
 */
class ProjectileMovementComponent : public EntityComponent
{

public:
    ProjectileMovementComponent(const AudioStore& audioStore,
            AudioSystem& audioSystem,
            MapNode targetNode,
            const EffectDef* impactEffectDef,
            std::shared_ptr<std::mt19937> randomizer,
            std::shared_ptr<const EntityFactory> entityFactory);

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    void update(int delta) override;
    // End EntityComponent override

    glm::vec2 getRenderOffset(int delta) const;

private:
    void onProjectileArrived();
    bool tryDamageEntityAtTarget(const AttackDef* attackDef) const;
    bool tryApplySplashDamage(const AttackDef* attackDef) const;
    void spawnImpactEffect() const;
    void playImpactSound(const std::string& soundName) const;

public:
    static const std::string key;

private:
    // TODO: This is only a rough estimate based on a few quick measurements
    static constexpr float splashDamageMultiplier = 0.25f;

    const AudioStore& audioStore;
    AudioSystem& audioSystem;
    std::shared_ptr<std::mt19937> randomizer;
    std::shared_ptr<const EntityFactory> entityFactory;

    EntityContainer* entityContainer = nullptr;

    MapNode targetNode;
    const EffectDef* impactEffectDef;
    int lifetime = 0;
    int timeElapsed = 0;
    float visualDistanceMultiplier = 0.f;
};

}  // namespace Rival
