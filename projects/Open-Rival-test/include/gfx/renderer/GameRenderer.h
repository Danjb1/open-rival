/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub declaration for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

// The GameRenderer includes a lot of other dependencies related to rendering,
// so by replacing it with a test-only version that does not contain these dependencies
// we save ourselves from having to mock a lot of other files.

#pragma once

namespace Rival {

class Camera;
class PlayerState;
class PlayerStore;
class Rect;
class Resources;
class Window;
class World;
struct PlayerContext;

class GameRenderer
{
public:
    GameRenderer(const Window* window,
            const World& world,
            const PlayerStore& playerStore,
            const Camera& camera,
            const Rect& viewport,
            const Resources& res,
            const PlayerContext& playerContext);

    void render(int delta);
};

}  // namespace Rival
