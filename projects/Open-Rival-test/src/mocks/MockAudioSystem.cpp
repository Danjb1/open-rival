/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include "audio/AudioSystem.h"

namespace Rival {

AudioSystem::AudioSystem()
    : midiActive(false)
    , soundActive(false)
{
}

AudioSystem::~AudioSystem()
{
    setMidiActive(false);
    setSoundActive(false);
}

///////////////////////////////////////////////////////////////////////////
// MIDI playback
///////////////////////////////////////////////////////////////////////////

void AudioSystem::midiThreadLoop() {}

void AudioSystem::startMidiSystem() {}

void AudioSystem::destroyMidiSystem() {}

void AudioSystem::setMidiActive(bool active)
{
    midiActive = active;
}

void AudioSystem::setSoundActive(bool active)
{
    soundActive = active;
}

void AudioSystem::playMidi(MidiFile midi) {}

///////////////////////////////////////////////////////////////////////////
// WAV playback
///////////////////////////////////////////////////////////////////////////

void AudioSystem::playSound(SoundSource source) {}

void AudioSystem::cleanUpPlayedSounds() {}

bool AudioSystem::isSoundPlaying(ALuint /*sourceId*/) const
{
    return true;
}

}  // namespace Rival
