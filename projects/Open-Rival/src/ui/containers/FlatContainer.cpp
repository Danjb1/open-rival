#include "ui/containers/FlatContainer.h"

#include <algorithm>  // std::max

namespace Rival {

FlatContainer::FlatContainer(FlatDirection direction, int spacing)
    : direction(direction)
    , spacing(spacing)
{
}

glm::ivec2 FlatContainer::getContentMinSize() const
{
    const int mainAxis = getMainAxis();
    const int crossAxis = getCrossAxis();
    const auto& allChildren = getAllChildren();

    glm::ivec2 contentMinSize { 0, 0 };

    for (const auto& child : allChildren)
    {
        glm::ivec2 childMinSize = child->getRequiredSize();
        contentMinSize[mainAxis] += childMinSize[mainAxis];
        contentMinSize[crossAxis] = std::max(contentMinSize[crossAxis], childMinSize[crossAxis]);
    }

    if (!allChildren.empty())
    {
        contentMinSize[mainAxis] += spacing * (static_cast<int>(allChildren.size()) - 1);
    }

    return contentMinSize;
}

void FlatContainer::arrangeChildren()
{
    const int mainAxis = getMainAxis();
    const int crossAxis = getCrossAxis();
    const auto& allChildren = getAllChildren();
    const glm::ivec2 containerSize = getSize();

    // Find how much leftover space there is to share between stretching children
    int numStretchingChildren = 0;
    for (const auto& child : allChildren)
    {
        if (isStretchingAlongMainAxis(*child))
        {
            ++numStretchingChildren;
        }
    }

    int leftoverSpace = std::max(0, containerSize[mainAxis] - getContentMinSize()[mainAxis]);
    int extraPerChild = numStretchingChildren > 0 ? leftoverSpace / numStretchingChildren : 0;

    // When the space doesn't divide evenly, the first few stretching children get an extra pixel each
    int remainder = numStretchingChildren > 0 ? leftoverSpace % numStretchingChildren : 0;

    int nextPosition = 0;

    for (const auto& child : allChildren)
    {
        int slotLength = child->getRequiredSize()[mainAxis];
        if (isStretchingAlongMainAxis(*child))
        {
            slotLength += extraPerChild;
            if (remainder > 0)
            {
                ++slotLength;
                --remainder;
            }
        }

        glm::ivec2 slotPosition { 0, 0 };
        slotPosition[mainAxis] = nextPosition;

        glm::ivec2 slotSize { 0, 0 };
        slotSize[mainAxis] = slotLength;
        slotSize[crossAxis] = containerSize[crossAxis];

        arrangeChild(*child, slotPosition, slotSize);

        nextPosition += slotLength + spacing;
    }
}

int FlatContainer::getMainAxis() const
{
    return direction == FlatDirection::Horizontal ? 0 : 1;
}

int FlatContainer::getCrossAxis() const
{
    return direction == FlatDirection::Horizontal ? 1 : 0;
}

bool FlatContainer::isStretchingAlongMainAxis(const UIElement& child) const
{
    ResizeMode resizeMode =
            direction == FlatDirection::Horizontal ? child.getHorizontalResizeMode() : child.getVerticalResizeMode();
    return resizeMode == ResizeMode::Stretch;
}

}  // namespace Rival
