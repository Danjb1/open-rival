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

void SDL_SetMainReady(void) {}

int SDL_Init(Uint32 /*flags*/)
{
    return 0;
}

void SDL_Quit(void) {}

void SDL_FreeSurface(SDL_Surface* /*surface*/) {}

int SDL_GL_SetAttribute(SDL_GLattr /*attr*/, int /*value*/)
{
    return 0;
}

int SDL_ShowCursor(int /*toggle*/)
{
    return 0;
}

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

void SDL_Delay(Uint32 /*ms*/) {}

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

const Uint8* SDL_GetKeyboardState(int* /*numkeys*/)
{
    return nullptr;
}

SDL_Scancode SDL_GetScancodeFromKey(SDL_Keycode /*key*/)
{
    return SDL_SCANCODE_UNKNOWN;
}

namespace MockSDL {
int ticks = 0;
int mouseX = 0;
int mouseY = 0;
}  // namespace MockSDL
