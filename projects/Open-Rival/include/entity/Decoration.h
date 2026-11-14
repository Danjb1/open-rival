#pragma once

#include "entity/Entity.h"

namespace Rival {

/** A Decoration object that can be added to the world. */
class Decoration : public Entity
{
public:
    static constexpr EntityType staticEntityType = EntityType::Decoration;

public:
    Decoration()
        : Entity(staticEntityType, /* isBig = */ false)
    {
    }

    std::string getDebugName() const override;
};

}  // namespace Rival
