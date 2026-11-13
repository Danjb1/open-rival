#pragma once

#include "game/GameInterface.h"
#include "gfx/opengl/renderable/SpriteRenderable.h"
#include "input/Input.h"
#include "ui/Cursor.h"
#include "window/Window.h"

namespace Rival {

class TextureStore;

class CursorRenderer
{
public:
    CursorRenderer(const TextureStore& textureStore, const Window* window, const Input& input);
    void render(const CursorDef& cursorDef, float timeSinceLastRender);

private:
    int animateCursor(const CursorDef& cursorDef, float timeSinceLastRender);

private:
    // Determines the speed of the cursor animation (seconds per image)
    static constexpr float timePerCursorFrame = 0.06f;

    const TextureStore& textureStore;
    const Window* window;
    const Input& input;

    GameInterface::UiImage cursorImage;
    SpriteRenderable cursorRenderable;
    CursorDef prevCursor = Cursor::arrow;

    /** Time spent displaying the current cursor (seconds). */
    float cursorAnimTime = 0;
};

}  // namespace Rival
