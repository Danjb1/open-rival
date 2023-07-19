/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include "mocks/MockSDL.h"

#define SDL_MAIN_HANDLED
#pragma warning(push)
#pragma warning(disable : 26819)
#include <SDL.h>
#pragma warning(pop)

int SDL_GL_SetSwapInterval(int /*i*/)
{
    return 0;
}

const char* SDL_GetError(void)
{
    return 0;
}

Uint32 SDL_GetTicks(void)
{
    return MockSDL::ticks;
}

void SDL_Delay(Uint32 /*ms*/)
{
    // Do nothing!
}

int SDL_PollEvent(SDL_Event* /*event*/)
{
    return 0;
}

Uint32 SDL_GetMouseState(int* x, int* y)
{
    *x = MockSDL::mouseX;
    *y = MockSDL::mouseY;
    return 0;
}

namespace MockSDL {
int ticks = 0;
int mouseX = 0;
int mouseY = 0;
}  // namespace MockSDL
