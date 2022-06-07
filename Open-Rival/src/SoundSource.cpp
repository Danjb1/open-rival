#include "pch.h"

#include "SoundSource.h"

namespace Rival {

    SoundSource::SoundSource(const WaveFile& waveFile)
        : waveFile(waveFile)
    {
    }

    SoundSource::SoundSource(const WaveFile& waveFile, std::vector<float> position)
        : waveFile(waveFile)
        , position(position)
        , positionRelativeToListener(false)
    {
    }

}  // namespace Rival
