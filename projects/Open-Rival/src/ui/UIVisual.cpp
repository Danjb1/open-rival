#include "ui/UIVisual.h"

#include "gfx/TextureAtlas.h"

namespace Rival {

glm::ivec2 getNaturalSize(const UIVisual& visual)
{
    if (const ImageVisual* image = std::get_if<ImageVisual>(&visual))
    {
        return { image->atlas->getImageWidth(image->imageKey), image->atlas->getImageHeight(image->imageKey) };
    }

    // Boxes have no natural size
    return { 0, 0 };
}

}  // namespace Rival
