#pragma once

#include "entity/Entity.h"

namespace Rival {

/** A Palisade that can be added to the world. */
class Palisade : public Entity
{
public:
    static constexpr EntityType staticEntityType = EntityType::Palisade;
    static constexpr int width = 1;
    static constexpr int height = 1;

public:
    Palisade(bool isGrate)
        : Entity(staticEntityType, width, height)
        , isGrate(isGrate)
    {
    }

private:
    bool isGrate;
};

}  // namespace Rival
