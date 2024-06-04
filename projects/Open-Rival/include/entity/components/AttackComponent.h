#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "entity/components/EntityComponent.h"
#include "entity/components/MovementListener.h"
#include "entity/components/UnitAnimationComponent.h"
#include "game/MapUtils.h"
#include "utils/CollectionUtils.h"

namespace Rival {

class AttackListener;
class AudioStore;
class AudioSystem;
class Entity;
class FacingComponent;
class MovementComponent;
class World;

enum class AttackState : std::uint8_t
{
    None,
    WaitingForIdle,
    Attacking,
    Cooldown
};

/**
 * Component that allows an entity to attack.
 */
class AttackComponent
    : public EntityComponent
    , public AnimationListener
{
public:
    AttackComponent(const AudioStore& audioStore, AudioSystem& audioSystem);

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World*) override;
    void destroy() override;
    void update(int delta) override;
    // End EntityComponent override

    // Begin AnimationListener override
    void onAnimationFinished(UnitAnimationType animType) override;
    // End AnimationListener override

    void setTarget(std::weak_ptr<Entity> newTarget);

    void addListener(std::weak_ptr<AttackListener> listener);
    void removeListener(std::weak_ptr<AttackListener> listener);

private:
    void deliverAttack();
    void switchToNewTarget();
    void updateCooldown(int delta);
    bool isInRange(const MapNode& node) const;
    void requestAttack(std::shared_ptr<Entity> targetEntity);
    void startAttack(std::shared_ptr<Entity> targetEntity);
    void moveToTarget(const MapNode& node);

public:
    static const std::string key;

private:
    const AudioStore& audioStore;
    AudioSystem& audioSystem;

    WeakPtrSet<AttackListener> listeners;

    /** Target of the current attack. */
    std::weak_ptr<Entity> weakTargetEntity;
    /** Requested target (next attack). */
    std::weak_ptr<Entity> weakRequestedTargetEntity;

    std::weak_ptr<MovementComponent> weakMovementComp;
    std::weak_ptr<FacingComponent> weakFacingComp;
    std::weak_ptr<UnitAnimationComponent> weakAnimationComp;

    AttackState attackState = AttackState::None;

    float cooldownTimeElapsed = 0;
    float cooldownDuration = 200;  // TODO: Depends on unit's attack speed?
};

}  // namespace Rival
