#ifndef MIDI_PLAYER_H
#define MIDI_PLAYER_H

#include <mutex>

#include "MidiFile.h"
#include "RtMidi.h"

namespace Rival {

    class MidiPlayer {
    public:
        // Throws a RtMidiError if a MIDI system initialization error occurs
        MidiPlayer();

        /**
         * Plays the given MIDI file synchronously from start to finish.
         */
        void play(MidiFile midi);

        /**
         * Stops the current track.
         *
         * This may block the calling thread for a few milliseconds as it has
         * to wait for the next scheduled MIDI command to finish.
         */
        void stop();

    private:
        std::mutex playingMutex;
        RtMidiOut midiOut;
        bool stopped;

        void chooseMidiPort();

        int MidiPlayer::getMessageSize(uint8_t eventId) const;
    };

}  // namespace Rival

#endif  // MIDI_PLAYER_H
