#include "pch.h"

#include "Animations.h"

namespace Rival {

Animation::Animation(int startIndex, int endIndex, int msPerFrame)
    : startIndex(startIndex)
    , endIndex(endIndex)
    , msPerFrame(msPerFrame)
{
}

}  // namespace Rival
