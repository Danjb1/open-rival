#pragma once

#include "game/Animations.h"

namespace Rival {

/** Interface that allows a class to be notified of animation events. */
template <typename T>
class AnimationListener
{
public:
    virtual void onAnimationFinished(T animType) = 0;
};

}  // namespace Rival
