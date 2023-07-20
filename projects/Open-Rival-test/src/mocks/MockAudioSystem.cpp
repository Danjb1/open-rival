/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include "audio/AudioSystem.h"

namespace Rival {

AudioSystem::AudioSystem() {}

AudioSystem::~AudioSystem() {}

///////////////////////////////////////////////////////////////////////////
// MIDI playback
///////////////////////////////////////////////////////////////////////////

void AudioSystem::setMidiActive(bool /*active*/) {}

void AudioSystem::setSoundActive(bool /*active*/) {}

void AudioSystem::playMidi(MidiFile midi) {}

///////////////////////////////////////////////////////////////////////////
// WAV playback
///////////////////////////////////////////////////////////////////////////

void AudioSystem::playSound(SoundSource source) {}

bool AudioSystem::isSoundPlaying(ALuint /*sourceId*/) const
{
    return true;
}

std::unordered_map<ALuint, SoundSource> AudioSystem::getPlayedSounds() const
{
    return {};
}

}  // namespace Rival
