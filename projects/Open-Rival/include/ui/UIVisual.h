#pragma once

#include <glm/vec4.hpp>

#include <variant>

namespace Rival {

/** A solid-coloured box that fills the element. */
struct BoxVisual
{
    /** RGBA, in the range 0-1. */
    glm::vec4 color { 1.f, 1.f, 1.f, 1.f };

    bool operator==(const BoxVisual& other) const = default;
};

/** Describes how a UIElement should be drawn, independent of any graphics API. */
using UIVisual = std::variant<BoxVisual>;

}  // namespace Rival
