#include "pch.h"
#include "AudioSystem.h"

#include <condition_variable>

#include "MidiPlayer.h"

namespace Rival {

    AudioSystem::AudioSystem()
        : midiActive(false) {}

    AudioSystem::~AudioSystem() {
        setMidiActive(false);
    }

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

    void AudioSystem::setMidiActive(bool active) {
        if (midiActive == active) {
            // Nothing to do!
            return;
        }

        if (midiActive) {
            // Kill the MIDI thread
            midiActive = active;
            midiPlayer.stop();
            midiReadyCondition.notify_one();
            midiThread.join();
        } else {
            // Start the MIDI thread
            midiActive = active;
            midiThread = std::thread(&AudioSystem::midiThreadLoop, this);
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
