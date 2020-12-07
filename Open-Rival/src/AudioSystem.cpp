#include "pch.h"
#include "AudioSystem.h"

#include "MidiPlayer.h"

namespace Rival {

    AudioSystem::AudioSystem()
        : midiActive(false) {}

    AudioSystem::~AudioSystem() {
        setMidiActive(false);
    }

    ///////////////////////////////////////////////////////////////////////////
    // MIDI playback
    ///////////////////////////////////////////////////////////////////////////

    void AudioSystem::midiThreadLoop() {
        std::unique_lock<std::mutex> midiReadyLock(midiReadyMutex);

        while (midiActive) {
            // Wait until we have a MIDI file to play
            while (midiActive && currentMidiTrack.isEmpty()) {
                midiReadyCondition.wait(midiReadyLock);
            }

            // Play the track from start to finish
            midiPlayer.play(currentMidiTrack);
        }
    }

    void AudioSystem::startMidiThread() {
        midiActive = true;
        midiPlayer.init();
        midiThread = std::thread(&AudioSystem::midiThreadLoop, this);
    }

    void AudioSystem::killMidiThread() {
        midiActive = false;
        midiPlayer.stop();
        midiReadyCondition.notify_one();
        midiThread.join();
    }

    void AudioSystem::setMidiActive(bool active) {
        if (midiActive == active) {
            // Nothing to do!
            return;
        }

        if (active) {
            startMidiThread();
        } else {
            killMidiThread();
        }
    }

    void AudioSystem::playMidi(MidiFile midi) {
        if (!midiActive) {
            return;
        }

        // Get ready to play the new track
        currentMidiTrack = midi;

        // Stop the current MIDI track (but our MIDI thread is still active!)
        midiPlayer.stop();

        // Notify our MIDI thread of the new track
        midiReadyCondition.notify_one();
    }

}  // namespace Rival
