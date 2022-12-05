#pragma once

#include <AL/al.h>

#include "SoundSource.h"
#include "WaveFile.h"

namespace Rival { namespace AudioUtils {

void initAL();

void destroyAL();

ALuint playSound(const SoundSource& source);

}}  // namespace Rival::AudioUtils
