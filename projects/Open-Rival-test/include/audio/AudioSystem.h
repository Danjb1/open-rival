/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub declaration for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#pragma once

#include <AL/al.h>

#include <unordered_map>

#include "audio/MidiFile.h"
#include "audio/SoundSource.h"

namespace Rival {

class AudioSystem
{
public:
    AudioSystem();
    ~AudioSystem();

    // Disable moving / copying
    AudioSystem(const AudioSystem& other) = delete;
    AudioSystem(AudioSystem&& other) = delete;
    AudioSystem& operator=(const AudioSystem& other) = delete;
    AudioSystem& operator=(AudioSystem&& other) = delete;

    ///////////////////////////////////////////////////////////////////////
    // MIDI playback
    ///////////////////////////////////////////////////////////////////////

public:
    void setMidiActive(bool active);

    void playMidi(MidiFile midi);

    void setSoundActive(bool active);

    void playSound(SoundSource source);

    bool isSoundPlaying(ALuint sourceId) const;

    std::unordered_map<ALuint, SoundSource> getPlayedSounds() const;
};

}  // namespace Rival
