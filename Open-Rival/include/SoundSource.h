#ifndef SOUND_SOURCE_H
#define SOUND_SOURCE_H

#include <vector>

#include "WaveFile.h"

namespace Rival {

    class SoundSource {
    public:
        const WaveFile& waveFile;
        float pitch { 1.0f };
        float gain { 1.0f };
        std::vector<float> position { 0, 0, 0 };
        std::vector<float> velocity { 0, 0, 0 };
        bool looping { false };
        bool positionRelativeToListener { true };

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

#endif  // SOUND_SOURCE_H
