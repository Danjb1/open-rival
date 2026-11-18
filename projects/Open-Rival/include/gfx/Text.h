#pragma once

#include <memory>
#include <string>

#include "gfx/Color.h"

namespace Rival {

class Font;

/**
 * A portion of text with a configurable color.
 */
struct TextSpan
{
    std::string text;
    Color color;

    bool operator==(const TextSpan& other) const = default;
};

/**
 * Properties that control how some text should be rendered.
 */
struct TextProperties
{
    /**
     * Font to use when rendering the text.
     */
    std::shared_ptr<const Font> font;

    /**
     * Scale at which to render the text.
     *
     * This is a fraction of the default font size.
     */
    float scale = 1.0f;

    /**
     * Whether the text should have a shadow.
     */
    bool hasShadow = true;

    bool operator==(const TextProperties& other) const = default;
};

}  // namespace Rival
