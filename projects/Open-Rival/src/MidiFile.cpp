#include "MidiFile.h"

namespace Rival {

MidiFile::MidiFile(midi_container midiContainer)
{
    // Extract MIDI events from container, ready for streaming
    unsigned long subsong = 0;
    system_exclusive_table p_system_exclusive;
    unsigned long loop_start = 0;
    unsigned long loop_end = 0;
    unsigned clean_flags = 0;
    midiContainer.serialize_as_stream(subsong, events, p_system_exclusive, loop_start, loop_end, clean_flags);
}

bool MidiFile::isEmpty() const
{
    return events.size() == 0;
}

const std::vector<midi_stream_event>& MidiFile::getEvents() const
{
    return events;
}

}  // namespace Rival
