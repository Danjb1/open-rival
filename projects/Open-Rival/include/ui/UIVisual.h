#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <memory>
#include <string>
#include <variant>

namespace Rival {

class TextureAtlas;

/** A solid-coloured box that fills the element. */
struct BoxVisual
{
    /** RGBA, in the range 0-1. */
    glm::vec4 color { 1.f, 1.f, 1.f, 1.f };

    bool operator==(const BoxVisual& other) const = default;
};

/** An image from a texture atlas, stretched to fill the element. */
struct ImageVisual
{
    /** Once an element is showing an image, it should not switch to an image from a different atlas. */
    std::shared_ptr<const TextureAtlas> atlas;
    std::string imageKey;

    bool operator==(const ImageVisual& other) const = default;
};

/** Describes how a UIElement should be drawn, independent of any graphics API. */
using UIVisual = std::variant<BoxVisual, ImageVisual>;

/** Gets the size at which a visual would naturally be drawn, e.g. the size of an image. */
glm::ivec2 getNaturalSize(const UIVisual& visual);

}  // namespace Rival
