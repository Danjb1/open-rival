#pragma once

namespace Rival {

struct MapNode;

/**
 * Interface used to listen to movements.
 */
class MovementListener
{
public:
    /** Called when a unit starts moving between tiles. */
    virtual void onUnitMoveStart(const MapNode* nextNode) = 0;

    /** Called when a unit pauses movement to wait for an obstruction to clear. */
    virtual void onUnitPaused() = 0;

    /** Called when a unit stops moving altogether. */
    virtual void onUnitStopped() = 0;
};

}  // namespace Rival
