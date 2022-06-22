#pragma once

#include <AL/alc.h>

#include "SoundSource.h"
#include "WaveFile.h"

namespace Rival { namespace AudioUtils {

void initAL();

void destroyAL();

void playSound(const SoundSource& source);

}}  // namespace Rival::AudioUtils
