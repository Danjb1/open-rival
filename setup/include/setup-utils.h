#ifndef SETUP_UTILS_H
#define SETUP_UTILS_H

#include <stdint.h>
#include <string>

#include "Image.h"
#include "Palette.h"

namespace Rival { namespace Setup {

    bool createDirectory(const std::string dirName);

    int nextPowerOf2(int v);

    void writeImage(Image& image, const Palette::Palette& palette, const std::string filename);

    /**
     * Produces a zero-padded string representation of a number.
     */
    std::string zeroPad(int value, int numDigits);

}}  // namespace Rival::Setup

#endif  // SETUP_UTILS_H
