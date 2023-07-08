#pragma once

#include "application/Window.h"
#include "game/GameInterface.h"
#include "gfx/renderable/SpriteRenderable.h"
#include "ui/Cursor.h"

namespace Rival {

class TextureStore;

class CursorRenderer
{
public:
    CursorRenderer(const TextureStore& textureStore, const Window* window);
    void render(const CursorDef& cursorDef, int delta);

private:
    int animateCursor(const CursorDef& cursorDef, int delta);

private:
    // Determines the speed of the cursor animation
    static constexpr int msPerCursorFrame = 60;

    const TextureStore& textureStore;
    const Window* window;

    GameInterface::UiImage cursorImage;
    SpriteRenderable cursorRenderable;
    CursorDef prevCursor = Cursor::arrow;
    int cursorAnimTime = 0;
};

}  // namespace Rival
