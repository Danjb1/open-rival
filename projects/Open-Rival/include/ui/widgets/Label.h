#pragma once

#include "gfx/Text.h"
#include "ui/UIElement.h"

namespace Rival {

/** Element that displays a single line of text. By default, it is the size of the text. */
class Label : public UIElement
{
public:
    Label(const TextSpan& span, const TextProperties& properties);

    void setText(const TextSpan& span, const TextProperties& properties);
};

}  // namespace Rival
