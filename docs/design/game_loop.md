# Open-Rival

## Game Loop

 - Rendering should be synced to refresh rate, if possible
    - For now, let's assume vsync is always enabled

 - If vsync is disabled for whatever reason, cap the FPS manually
    - Use `SDL_Delay(1)` to sleep for (at least) 1ms

 - Game logic should run at a fixed rate, regardless of refresh rate
    - If render rate is slower than the logic, execute logic multiple times per render
    - If render rate is faster than the logic, we should skip some render operations
        - There is no point rendering if nothing has changed!
        - Exception: if using vsync we should always render (?)

 - Calling `SDL_GL_SwapWindow` will swap the buffers at the next acceptable "swap interval"
    - With vsync enabled, this will be in-sync with the screen's refresh rate
    - With vsync disabled, this will happen immediately
    - For best performance, it is important that we never miss an interval!
    - This means we always have to call `SDL_GL_SwapWindow` before the swap interval occurs

See:
https://gamedev.stackexchange.com/questions/151877/handling-variable-frame-rate-in-sdl2
https://gamedev.stackexchange.com/questions/79490/should-i-cap-the-frame-rate-in-sdl

### Fixed Timestep

The logic should run at a fixed rate - but what does this mean?

Simply put, if our timestep is 16ms (~60fps), the logic should always behave as
though 16ms have passed, even if that is not strictly the case. This prevents
strange behaviour arising from extreme delta times, and it ensures the logic is
deterministic, which is useful for replays.

Obviously if the logic *thinks* it's running at 60fps but we are only actually
updating it 30 times a second, the game will appear to run slow (units will move
half as fast as they should). Similarly, if the logic thinks it's running at
30fps but we are updating it 60 times a second, the game will appear to run fast
(units will move twice as fast as they should).

Therefore, for best results, we should try to update the logic at the "correct"
rate. In practice it is difficult to keep the timestep entirely consistent, but
at the very least we should update the logic the correct number of times per
second.

### Animations

For a 3d game with continuous animations, it makes sense to tie animations to
the refresh rate because time has a bearing on the precise positioning of the
vertices.

For 2d sprites with discrete frames, it makes little difference where the
animation is updated. It is much simpler to handle this in the logic, since
that is where the animations will be triggered.

### Example

Let's say we want our logic to run at 60fps, vsync is enabled, and we cannot
make any guarantees about the screen's refresh rate.

This is a simple *fixed timestep* loop based on:
https://gamedev.stackexchange.com/questions/151877/handling-variable-frame-rate-in-sdl2

---

    Uint32 timeStepMs = 1000 / 60;
    Uint32 nextUpdateDue = SDL_GetTicks();

    while (running){
      Uint32 frameStartTime = SDL_GetTicks();

      // Is the next update due?
      if (vsyncEnabled || nextUpdateDue <= frameStartTime){

        // Update the game logic, as many times as necessary to keep it in-sync
        // with the refresh rate.
        //
        // For example:
        //  - For a 30Hz monitor, this will run twice per render.
        //  - For a 60Hz monitor, this will run once per render.
        //  - For a 120Hz monitor, this will run every other render.
        //
        // If vsync is disabled, this should run once per render.
        while (nextUpdateDue <= frameStartTime) {
          update(timeStepMs);
          nextUpdateDue += timeStepMs;
        }

        // Render the game once per iteration.
        // With vsync enabled, this matches the screen's refresh rate.
        // Otherwise, this matches our target FPS.
        render();
        
        // If vsync is enabled this will wait until the next swap interval
        SDL_GL_SwapWindow();

      } else {
        // Next update is not yet due.
        // Sleep for the shortest possible time, so as not to risk overshooting!
        SDL_Delay(1);
      }
    }

---

Notes:

 - We could limit the number of times the logic loop runs.
    This would mean that if a lagspike occurs, the game would slow down instead
    of running the logic 100s of times in a single frame and potentially causing
    strange behaviour.

 - Since the logic timestep is constant, there is really no need to pass it as
    a parameter; it can live somewhere as a global constant.
