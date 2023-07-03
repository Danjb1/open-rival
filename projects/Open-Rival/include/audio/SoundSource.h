#pragma once

#include <vector>

#include "Unit.h"
#include "audio/WaveFile.h"

namespace Rival {

struct SoundSource
{
    const WaveFile& waveFile;
    float pitch { 1.0f };
    float gain { 1.0f };
    std::vector<float> position { 0, 0, 0 };
    std::vector<float> velocity { 0, 0, 0 };
    bool looping { false };
    bool positionRelativeToListener { true };

    /**
     * Unit type that generated the sound, if applicable.
     */
    Unit::Type unitType = Unit::Type::Invalid;

    /**
     * Creates a new SoundSource positioned at the listener.
     */
    SoundSource(const WaveFile& waveFile);

    /**
     * Creates a new SoundSource at the given position.
     */
    SoundSource(const WaveFile& waveFile, std::vector<float> position);
};

}  // namespace Rival
