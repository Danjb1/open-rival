#include "pch.h"

#include "PortraitComponent.h"

namespace Rival {

const std::string PortraitComponent::key = "portrait";

PortraitComponent::PortraitComponent(int portraitId)
    : EntityComponent(key)
    , portraitId(portraitId)
{
}

}  // namespace Rival
