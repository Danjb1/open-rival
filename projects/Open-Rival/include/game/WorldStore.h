#pragma once

namespace Rival {

class World;

/**
 * Interface providing access to the world.
 */
class WorldStore
{
public:
    virtual World& getWorld() = 0;
};

}  // namespace Rival
