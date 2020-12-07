#ifndef MIDI_FILE_H
#define MIDI_FILE_H

#include <vector>

#include "MidiContainer.h"

namespace Rival {

    class MidiFile {
    public:
        MidiFile() {}
        MidiFile(midi_container midiContainer);

        bool isEmpty() const;

        const std::vector<midi_stream_event>& getEvents() const;

    private:
        std::vector<midi_stream_event> events;
    };

}  // namespace Rival

#endif  // MIDI_FILE_H
