#include "pch.h"

#include "AudioSystem.h"

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

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

}  // namespace Rival
