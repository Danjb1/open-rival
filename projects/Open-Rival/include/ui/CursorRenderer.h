#pragma once

#include "Cursor.h"
#include "GameInterface.h"
#include "SpriteRenderable.h"
#include "Window.h"

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
