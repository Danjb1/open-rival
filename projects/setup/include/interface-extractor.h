#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "BinaryFileReader.h"
#include "Palette.h"

namespace Rival { namespace Setup {

class InterfaceExtractor
{
public:
    InterfaceExtractor(std::string inputFile);

    void extractImages(std::string outputDir);

private:
    int index = 0;
    Rival::BinaryFileReader reader;

    void extractImage(const std::string& outputDir, const Palette::Palette& palette);
};

}}  // namespace Rival::Setup
