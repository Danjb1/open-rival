/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include "application/Resources.h"
#include "audio/AudioSystem.h"

namespace Rival {

AudioSystem::AudioSystem() {}

AudioSystem::~AudioSystem() {}

///////////////////////////////////////////////////////////////////////////
// MIDI playback
///////////////////////////////////////////////////////////////////////////

void AudioSystem::setMidiActive(bool /*active*/) {}

void AudioSystem::setSoundActive(bool /*active*/) {}

void AudioSystem::playMidi(std::shared_ptr<const MidiFile> /*midi*/) {}

///////////////////////////////////////////////////////////////////////////
// WAV playback
///////////////////////////////////////////////////////////////////////////

void AudioSystem::playSound(const AudioStore&, int, SoundConfig) {}

void AudioSystem::playSound(SoundSource /*source*/) {}

bool AudioSystem::isSoundPlaying(ALuint /*sourceId*/) const
{
    return true;
}

std::unordered_map<ALuint, SoundSource> AudioSystem::getPlayedSounds() const
{
    return {};
}

}  // namespace Rival
