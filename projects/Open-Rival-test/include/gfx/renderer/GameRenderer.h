/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub declaration for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

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
    GameRenderer(
            const Window* window,
            const World& world,
            const PlayerStore& playerStore,
            const Camera& camera,
            const Rect& viewport,
            const Resources& res,
            const PlayerContext& playerContext);

    void render(int delta);
};

}  // namespace Rival
