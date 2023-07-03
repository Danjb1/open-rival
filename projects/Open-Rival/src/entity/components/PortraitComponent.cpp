#include "entity/components/PortraitComponent.h"

namespace Rival {

const std::string PortraitComponent::key = "portrait";

PortraitComponent::PortraitComponent(int portraitId)
    : EntityComponent(key)
    , portraitId(portraitId)
{
}

}  // namespace Rival
