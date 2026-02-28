#pragma once

#include <string>

#include "entity/components/EntityComponent.h"
#include "entity/components/MovementListener.h"
#include "game/MapUtils.h"

namespace Rival {

class MovementComponent;
struct MapNode;

/**
 * Interface used to listen to changes in facing.
 */
class FacingListener
{
public:
    virtual void facingChanged(Facing newFacing) = 0;
};

/**
 * Component that manages an Entity's facing.
 */
class FacingComponent
    : public EntityComponent
    , public MovementListener
{

public:
    FacingComponent(Facing initialFacing);

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    void destroy() override;
    // End EntityComponent override

    // Begin MovementComponent override
    void onUnitMoveStart(const MapNode* nextNode) override;
    void onUnitPaused() override;
    void onUnitStopped() override;
    // End MovementComponent override

    void setListener(FacingListener* listener);

    void setFacing(Facing newFacing);

    Facing getFacing() const
    {
        return facing;
    }

    int getFacingIndex() const
    {
        return static_cast<int>(facing);
    }

    /** Gets the facing index to use for the death animation, as this requires custom logic. */
    int getDeathFacingIndex() const;

    void rotateLeft();

    void rotateRight();

private:
    void notifyListener() const;

public:
    static const std::string key;

private:
    static constexpr int numFacings = 8;

    std::weak_ptr<MovementComponent> weakMovementComponent;

    Facing facing;

    FacingListener* listener { nullptr };
};

}  // namespace Rival
