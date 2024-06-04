#pragma once

#include <memory>
#include <vector>

#include "audio/Sounds.h"
#include "audio/WaveFile.h"
#include "game/UnitType.h"

namespace Rival {

/** A sound that can be played through the audio system. */
struct SoundSource
{
    std::shared_ptr<const WaveFile> waveFile;
    SoundConfig cfg;

    /** Creates a new SoundSource positioned at the listener. */
    SoundSource(std::shared_ptr<const WaveFile> waveFile, SoundConfig cfg = {});
};

}  // namespace Rival
