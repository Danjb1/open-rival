#pragma once

#include <AL/al.h>

#include "audio/SoundSource.h"
#include "audio/WaveFile.h"

namespace Rival { namespace AudioUtils {

void initAL();

void destroyAL();

ALuint playSound(const SoundSource& source);

}}  // namespace Rival::AudioUtils
