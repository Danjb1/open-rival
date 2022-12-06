#pragma once

#include <string>

#include "EntityComponent.h"

namespace Rival {

/**
 * Component that holds an entity's portrait ID.
 */
class PortraitComponent : public EntityComponent
{

public:
    static const std::string key;

    PortraitComponent(int portraitId);

    int getPortraitId() const
    {
        return portraitId;
    }

private:
    int portraitId;
};

}  // namespace Rival
