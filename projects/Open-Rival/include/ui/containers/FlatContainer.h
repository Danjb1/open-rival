#pragma once

#include <glm/vec2.hpp>

#include "ui/UIElement.h"

namespace Rival {

/** Direction in which a FlatContainer lays out its children. */
enum class FlatDirection
{
    Horizontal,
    Vertical
};

/**
 * UIElement that positions its children in a single row or column, in the order they were added.
 *
 * Any space left over along the container's direction is shared between the children that stretch in that direction.
 * If there are no such children, the leftover space is left empty after the last child.
 */
class FlatContainer : public UIElement
{
public:
    explicit FlatContainer(FlatDirection direction, int spacing = 0);

protected:
    glm::ivec2 getContentMinSize() const override;
    void arrangeChildren() override;

private:
    /** Gets the index of the vector component along this container's direction. */
    int getMainAxis() const;

    /** Gets the index of the vector component perpendicular to this container's direction. */
    int getCrossAxis() const;

    bool isStretchingAlongMainAxis(const UIElement& child) const;

    FlatDirection direction;

    /** Gap between consecutive children, in pixels. */
    int spacing;
};

}  // namespace Rival
