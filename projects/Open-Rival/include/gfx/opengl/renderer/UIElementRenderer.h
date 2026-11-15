#pragma once

#include "ui/UIRoot.h"

namespace Rival {

// TODO: Renderer that can render an entire UI.
// Later, we should rename this to UIRenderer (and deprecate the old one).
// MenuRenderer should also be deprecated.
class UIElementRenderer
{
public:
    void render(UIRoot* uiRoot);
};

}  // namespace Rival
