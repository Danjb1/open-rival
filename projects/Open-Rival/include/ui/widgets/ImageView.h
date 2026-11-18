#pragma once

#include "ui/UIElement.h"

namespace Rival {

/** Element that displays an image. By default, it is the size of the image. */
class ImageView : public UIElement
{
public:
    explicit ImageView(const ImageVisual& image);
};

}  // namespace Rival
