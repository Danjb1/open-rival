#pragma once

#include <string>

#include "entity/Entity.h"

namespace Rival {

/** A Palisade that can be added to the world. */
class Palisade : public Entity
{
public:
    static constexpr EntityType staticEntityType = EntityType::Palisade;

public:
    Palisade(bool isGrate)
        : Entity(staticEntityType, /* isBig = */ false)
        , isGrate(isGrate)
    {
    }

    std::string getDebugName() const override;

private:
    bool isGrate;
};

}  // namespace Rival
