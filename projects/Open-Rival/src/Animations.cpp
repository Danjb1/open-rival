#include "pch.h"

#include "Animations.h"

namespace Rival {

Animation::Animation(int startIndex, int endIndex, int msPerFrame, int facingStride)
    : startIndex(startIndex)
    , endIndex(endIndex)
    , msPerFrame(msPerFrame)
    , facingStride(facingStride)
{
}

}  // namespace Rival
