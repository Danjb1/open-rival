#include "pch.h"
#include "MockSDL.h"

int SDL_GL_SetSwapInterval(int i) {
    return 0;
}

const char* SDL_GetError(void) {
    return 0;
}

Uint32 SDL_GetTicks(void) {
    return MockSDL::ticks;
}

void SDL_Delay(Uint32 ms) {
    // Do nothing!
}

int SDL_PollEvent(SDL_Event* event) {
    return 0;
}
