#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#include <AL/alc.h>

#include "SoundSource.h"
#include "WaveFile.h"

namespace Rival {
namespace AudioUtils {

    void initAL();

    void destroyAL();

    void playSound(const SoundSource& source);

}}  // namespace Rival::AudioUtils

#endif  // AUDIO_UTILS_H
