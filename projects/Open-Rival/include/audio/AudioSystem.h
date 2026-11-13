#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "audio/MidiFile.h"
#include "audio/SoundSource.h"
#include "audio/Sounds.h"

namespace Rival {

class AudioStore;
class WaveFile;

/** Interface for a system responsible for playing music and sounds. */
class AudioSystem
{
public:
    AudioSystem() = default;
    virtual ~AudioSystem() = default;

    // Disable moving / copying
    AudioSystem(const AudioSystem& other) = delete;
    AudioSystem(AudioSystem&& other) = delete;
    AudioSystem& operator=(const AudioSystem& other) = delete;
    AudioSystem& operator=(AudioSystem&& other) = delete;

    ///////////////////////////////////////////////////////////////////////
    // MIDI playback
    ///////////////////////////////////////////////////////////////////////

    /** Enables or disables MIDI playback. */
    virtual void setMidiActive(bool active) = 0;

    /** Plays the given MIDI file.
     * Requires that MIDI playback is enabled. */
    virtual void playMidi(std::shared_ptr<const MidiFile> midi) = 0;

    ///////////////////////////////////////////////////////////////////////
    // WAV playback
    ///////////////////////////////////////////////////////////////////////

    /** Enables or disables sound playback. */
    virtual void setSoundActive(bool active) = 0;

    /** Prepares a list of sounds for future playback.
     * This must be called before any of these sounds can be played. */
    virtual void prepareSounds(std::vector<std::shared_ptr<const WaveFile>> sounds) = 0;

    /** Finds and plays the given sound.
     * Requires that sound playback is enabled. */
    virtual void playSound(const AudioStore& audioStore, int soundId, SoundConfig cfg = {}) = 0;

    /** Plays the given sound.
     * Requires that sound playback is enabled. */
    virtual void playSound(SoundSource source) = 0;

    /** Determines if the given sound source is currently playing. */
    virtual bool isSoundPlaying(SoundSourceId sourceId) const = 0;

    /** Gets the most recently played sounds.
     * Note that these sounds may already have finished playing. */
    virtual std::unordered_map<SoundSourceId, SoundSource> getPlayedSounds() const = 0;
};

}  // namespace Rival
