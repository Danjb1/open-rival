#pragma once

#include <functional>

#include "ui/UIElement.h"

namespace Rival {

/** Button element that emits an event when pressed. */
class Button : public UIElement
{
public:
    Button();

    /** Sets the function to call when this button is clicked. */
    void setOnClick(std::function<void()> callback);

    void onMouseEnter() override;
    void onMouseExit() override;
    bool onMouseMoveEvent(const MouseMoveEvent& evt) override;
    bool onMouseButtonEvent(const MouseButtonEvent& evt) override;

private:
    std::function<void()> onClick;
};

}  // namespace Rival
