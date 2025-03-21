#pragma once

#include <stdint.h>

#include <string>

#include "gfx/Image.h"
#include "gfx/Palette.h"

namespace Rival { namespace Setup {

bool createDirectory(const std::string dirName);

void writeImage(Image& image, const Palette::Palette& palette, const std::string filename);

/**
 * Produces a zero-padded string representation of a number.
 */
std::string zeroPad(int value, int numDigits);

}}  // namespace Rival::Setup
