#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include <mutex>
#include <thread>

#include "MidiFile.h"
#include "MidiPlayer.h"

namespace Rival {

    class AudioSystem {
    public:
        AudioSystem();
        ~AudioSystem();

        // Disable moving / copying
        AudioSystem(const AudioSystem& other) = delete;
        AudioSystem(AudioSystem&& other) = delete;
        AudioSystem& operator=(const AudioSystem& other) = delete;
        AudioSystem& operator=(AudioSystem&& other) = delete;

        void setMidiActive(bool active);

        void playMidi(MidiFile midi);

    private:
        bool midiActive;
        MidiPlayer midiPlayer;
        MidiFile currentMidiTrack;
        std::thread midiThread;
        std::condition_variable midiReadyCondition;
        std::mutex midiReadyMutex;

        void midiThreadLoop();
    };

}  // namespace Rival

#endif  // AUDIO_SYSTEM_H
