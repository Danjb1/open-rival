#include "audio/SoundSource.h"

namespace Rival {

SoundSource::SoundSource(std::shared_ptr<const WaveFile> waveFile, SoundConfig cfg)
    : waveFile(waveFile)
    , cfg(cfg)
{
}

}  // namespace Rival
