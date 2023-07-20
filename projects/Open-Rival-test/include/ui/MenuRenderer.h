/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub declaration for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#pragma once

#include "application/Resources.h"

namespace Rival {

class Rect;
class Resources;
class Window;

class MenuRenderer
{
public:
    MenuRenderer(const TextureStore& textureStore, const Window* window, const Rect& viewport);

    void render(int delta);
};

}  // namespace Rival
