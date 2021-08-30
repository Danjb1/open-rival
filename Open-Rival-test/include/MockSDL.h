#ifndef MOCK_SDL_h_
#define MOCK_SDL_h_

/*
 * This header should be included instead of the real SDL header in tests.
 *
 * This will pull in the required SDL headers, but provides mock implementations
 * of the required functionality.
 */

namespace MockSDL {
    extern int ticks;
    extern int mouseX;
    extern int mouseY;
}  // namespace MockSDL

#endif  // MOCK_SDL_h_
