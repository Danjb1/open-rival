#pragma once

#include <functional>

#include "gfx/Text.h"
#include "ui/UIElement.h"

namespace Rival {

class Label;

/** Button element that emits an event when pressed. By default, it is the size of its normal visual. */
class Button : public UIElement
{
public:
    Button(const UIVisual& newNormalVisual, const UIVisual& newPressedVisual);

    /** Sets the function to call when this button is clicked. */
    void setOnClick(std::function<void()> callback);

    /** Sets the text shown in the centre of this button, adding a Label if there isn't one already. */
    void setText(const TextSpan& span, const TextProperties& properties);

    void onMouseExit() override;
    bool onMouseMoveEvent(const MouseMoveEvent& evt) override;
    bool onMouseButtonEvent(const MouseButtonEvent& evt) override;

private:
    UIVisual normalVisual;
    UIVisual pressedVisual;
    std::function<void()> onClick;

    /** The Label created by setText, if any. */
    Label* label = nullptr;
};

}  // namespace Rival
