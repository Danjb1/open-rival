#pragma once

namespace Rival {

struct MapNode;

/**
 * Interface used to listen to movements.
 */
class MovementListener
{
public:
    virtual void onUnitMoveStart(const MapNode* nextNode) = 0;
    virtual void onUnitStopped() = 0;
};

}  // namespace Rival
