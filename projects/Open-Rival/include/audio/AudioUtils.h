#pragma once

#include <AL/al.h>

#include "audio/SoundSource.h"
#include "audio/WaveFile.h"

namespace Rival { namespace AudioUtils {

void initAL();

void destroyAL();

/** Creates a buffer for the given sound, which is reused each time the sound is played.
 * Returns the buffer ID. */
ALuint createBuffer(std::shared_ptr<const WaveFile> waveFile);

/** Destroys the given buffer. */
void destroyBuffer(ALuint buffer);

ALuint playSound(const SoundSource& source, ALuint buffer);

}}  // namespace Rival::AudioUtils
