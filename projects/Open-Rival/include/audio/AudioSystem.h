#pragma once

#include <AL/al.h>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>

#include "audio/MidiFile.h"
#include "audio/MidiPlayer.h"
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
    /**
     * Enables or disables MIDI playback.
     */
    void setMidiActive(bool active);

    /**
     * Plays the given MIDI file.
     *
     * Requires that MIDI playback is enabled.
     */
    void playMidi(MidiFile midi);

private:
    /**
     * Flag set when MIDI playback is enabled.
     */
    bool midiActive;

    /**
     * MidiPlayer used to play MIDI files synchronously in a background
     * thread.
     */
    MidiPlayer midiPlayer;

    /**
     * The MIDI track being played.
     *
     * If this is an empty MidiFile, the MIDI thread will wait for
     * `midiReadyCondition` to be notified.
     */
    MidiFile currentMidiTrack;

    /**
     * Background thread used to play MIDI files.
     */
    std::thread midiThread;

    /**
     * Condition used to wait / notify the MIDI thread.
     */
    std::condition_variable midiReadyCondition;

    /**
     * Mutex used to govern access to the current MIDI track.
     */
    std::mutex midiMutex;

    /**
     * Plays the current MIDI track on a loop, for as long as MIDI playback
     * is enabled.
     */
    void midiThreadLoop();

    /**
     * Causes the current thread to wait until `midiReadyCondition` is
     * notified, and the current MIDI track is set to a non-empty file.
     */
    void waitForMidi();

    /**
     * Initializes the MIDI device, and starts the MIDI thread.
     */
    void startMidiSystem();

    /**
     * Cleans up all MIDI resources.
     */
    void destroyMidiSystem();

    ///////////////////////////////////////////////////////////////////////
    // WAV playback
    ///////////////////////////////////////////////////////////////////////

public:
    /**
     * Enables or disables sound playback.
     */
    void setSoundActive(bool active);

    /**
     * Plays the given sound.
     *
     * Requires that sound playback is enabled.
     */
    void playSound(SoundSource source);

    /**
     * Determines if the given sound source is currently playing.
     */
    bool isSoundPlaying(ALuint sourceId) const;

    /**
     * Gets the most recently played sounds.
     *
     * Note that these sounds may already have finished playing.
     */
    std::unordered_map<ALuint, SoundSource> getPlayedSounds() const
    {
        return playedSounds;
    }

private:
    /**
     * Flag set when sound playback is enabled.
     */
    bool soundActive;

    /**
     * Queue of sounds waiting to be played.
     */
    std::queue<SoundSource> soundQueue;

    /**
     * Map of sounds that are currently playing.
     */
    std::unordered_map<ALuint, SoundSource> playedSounds;

    /**
     * Background thread used to play sound files.
     */
    std::thread soundThread;

    /**
     * Condition used to wait / notify the sound thread.
     */
    std::condition_variable soundReadyCondition;

    /**
     * Mutex used to govern access to the sound queue.
     */
    std::mutex soundQueueMutex;

    /**
     * Plays sounds from the queue, for as long as sound playback is
     * enabled.
     */
    void soundThreadLoop();

    /**
     * Causes the current thread to wait until `soundReadyCondition` is
     * notified, and the sound queue is not empty.
     */
    void waitForSound();

    /**
     * Plays the next sound from the queue.
     */
    void playNextSound();

    /**
     * Initializes the sound device, and starts the MIDI thread.
     */
    void startSoundSystem();

    /**
     * Cleans up all sound resources.
     */
    void destroySoundSystem();

    /**
     * Stops all sounds that are playing.
     */
    void stopAllSounds();

    void cleanUpPlayedSounds();
};

}  // namespace Rival
