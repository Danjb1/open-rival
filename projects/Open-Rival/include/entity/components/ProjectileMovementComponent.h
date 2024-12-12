#pragma once

#include <glm/vec2.hpp>

#include <memory>
#include <random>
#include <string>

#include "entity/components/EntityComponent.h"
#include "game/MapUtils.h"
#include "utils/EntityUtils.h"

namespace Rival {

class AudioStore;
class AudioSystem;

/**
 * Component that moves a projectile to its destination.
 */
class ProjectileMovementComponent : public EntityComponent
{

public:
    ProjectileMovementComponent(const AudioStore& audioStore,
            AudioSystem& audioSystem,
            MapNode target,
            std::shared_ptr<std::mt19937> randomizer);

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    void update(int delta) override;
    // End EntityComponent override

    glm::vec2 getRenderOffset(int delta) const;

private:
    void onProjectileArrived();

public:
    static const std::string key;

private:
    const AudioStore& audioStore;
    AudioSystem& audioSystem;
    std::shared_ptr<std::mt19937> randomizer;

    EntityContainer* entityContainer = nullptr;

    MapNode target;
    int lifetime = 0;
    int timeElapsed = 0;
    float visualDistanceMultiplier = 0.f;
};

}  // namespace Rival
