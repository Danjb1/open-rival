#include "pch.h"
#include "AudioSystem.h"

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

namespace Rival {

    AudioSystem::AudioSystem()
        : midiActive(false) {}

    AudioSystem::~AudioSystem() {
        setMidiActive(false);
    }

    ///////////////////////////////////////////////////////////////////////////
    // MIDI playback
    ///////////////////////////////////////////////////////////////////////////

    void AudioSystem::midiThreadLoop() {}

    void AudioSystem::startMidiThread() {}

    void AudioSystem::killMidiThread() {}

    void AudioSystem::setMidiActive(bool active) {
        midiActive = active;
    }

    void AudioSystem::playMidi(MidiFile midi) {}

}  // namespace Rival
