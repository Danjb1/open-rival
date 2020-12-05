#include "pch.h"
#include "MidiPlayer.h"

#include <chrono>
#include <stdexcept>

#include "RtMidi.h"

namespace Rival {

// Platform-dependent sleep routines
#ifdef _WIN32
#include <windows.h>
#define SLEEP(milliseconds) Sleep((DWORD) milliseconds)
#else  // Unix variants
#include <unistd.h>
#define SLEEP(milliseconds) usleep((unsigned long) (milliseconds * 1000.0))
#endif

    MidiPlayer::MidiPlayer() {
        chooseMidiPort();
    }

    void MidiPlayer::chooseMidiPort() {
        unsigned int nPorts = midiOut.getPortCount();
        if (nPorts == 0) {
            throw std::runtime_error("No MIDI output ports available!");
        }

        std::cout << "\nOpening MIDI port: " << midiOut.getPortName() << "\n";

        midiOut.openPort(0);
    }

    void MidiPlayer::play(MidiFile file) {
        std::chrono::milliseconds startTime =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch());

        const std::vector<midi_stream_event>& events = file.getEvents();

        for (auto const& evt : events) {
            std::vector<unsigned char> message;

            // Put each byte of the event into the message
            uint32_t eventValue = evt.m_event;
            message.push_back(eventValue & 0xFF);
            message.push_back((eventValue >> 8) & 0xFF);
            message.push_back((eventValue >> 16) & 0xFF);

            // Wait until the message is due
            std::chrono::milliseconds currentTime =
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch());
            auto timeElapsed = currentTime - startTime;
            long timeUntilMessage = evt.m_timestamp - timeElapsed.count();
            if (timeUntilMessage > 0) {
                SLEEP(timeUntilMessage);
            }

            // Send the message to our MIDI output
            midiOut.sendMessage(&message);
        }
    }

}  // namespace Rival
