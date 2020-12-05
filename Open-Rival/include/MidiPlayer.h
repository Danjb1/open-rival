#ifndef MIDI_PLAYER_H
#define MIDI_PLAYER_H

#include "MidiFile.h"

#include "RtMidi.h"

namespace Rival {

    class MidiPlayer {
    public:
        // Throws a RtMidiError if a MIDI system initialization error occurs
        MidiPlayer();

        void play(MidiFile midi);

    private:
        RtMidiOut midiOut;

        void chooseMidiPort();
    };

}  // namespace Rival

#endif  // MIDI_PLAYER_H
