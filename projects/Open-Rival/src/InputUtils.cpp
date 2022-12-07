#include "pch.h"

#include "InputUtils.h"

namespace Rival { namespace InputUtils {

bool isKeyDown(const SDL_Keycode keyCode)
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    SDL_Scancode scanCode = SDL_GetScancodeFromKey(keyCode);
    return currentKeyStates[scanCode];
}

}}  // namespace Rival::InputUtils
