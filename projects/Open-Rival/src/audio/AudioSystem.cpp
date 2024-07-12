#include "audio/AudioSystem.h"

#include <utility>

#include "application/Resources.h"
#include "audio/AudioUtils.h"
#include "audio/MidiPlayer.h"
#include "utils/LogUtils.h"

namespace Rival {

AudioSystem::AudioSystem()
    : midiActive(false)
    , soundActive(false)
{
}

AudioSystem::~AudioSystem()
{
    setMidiActive(false);
    setSoundActive(false);
}

///////////////////////////////////////////////////////////////////////////
// MIDI playback
///////////////////////////////////////////////////////////////////////////

void AudioSystem::midiThreadLoop()
{
    while (midiActive)
    {
        waitForMidi();
        midiPlayer.play(currentMidiTrack);
    }
}

void AudioSystem::waitForMidi()
{
    std::unique_lock<std::mutex> midiReadyLock(midiMutex);
    while (midiActive && (!currentMidiTrack || currentMidiTrack->isEmpty()))
    {
        midiReadyCondition.wait(midiReadyLock);
    }
}

void AudioSystem::setMidiActive(bool active)
{
    if (midiActive == active)
    {
        // Nothing to do!
        return;
    }

    if (active)
    {
        startMidiSystem();
    }
    else
    {
        destroyMidiSystem();
    }
}

void AudioSystem::startMidiSystem()
{
    midiActive = true;
    midiPlayer.init();
    midiThread = std::thread(&AudioSystem::midiThreadLoop, this);
}

void AudioSystem::destroyMidiSystem()
{
    midiActive = false;
    midiPlayer.stop();
    midiReadyCondition.notify_one();
    midiThread.join();
}

void AudioSystem::playMidi(std::shared_ptr<const MidiFile> midi)
{
    if (!midiActive)
    {
        return;
    }

    // Get ready to play the new track
    const std::scoped_lock<std::mutex> lock(midiMutex);
    currentMidiTrack = midi;

    // Stop the current MIDI track (but our MIDI thread is still active!)
    midiPlayer.stop();

    // Notify our MIDI thread of the new track
    midiReadyCondition.notify_one();
}

///////////////////////////////////////////////////////////////////////////
// WAV playback
///////////////////////////////////////////////////////////////////////////

void AudioSystem::soundThreadLoop()
{
    while (soundActive)
    {
        waitForSound();
        playNextSound();
    }
}

void AudioSystem::waitForSound()
{
    std::unique_lock<std::mutex> soundReadyLock(soundQueueMutex);
    while (soundActive && soundQueue.size() == 0)
    {
        soundReadyCondition.wait(soundReadyLock);
    }
}

void AudioSystem::playNextSound()
{
    const std::scoped_lock<std::mutex> lock(soundQueueMutex);
    if (soundQueue.empty())
    {
        return;
    }

    SoundSource& sound = soundQueue.front();
    try
    {
        ALint sourceId = AudioUtils::playSound(sound);
        soundQueue.pop();
        playedSounds.emplace(sourceId, sound);
    }
    catch (const std::runtime_error& e)
    {
        // Don't crash the game because of an audio error
        LOG_ERROR(e.what());
    }
}

void AudioSystem::setSoundActive(bool active)
{
    if (soundActive == active)
    {
        // Nothing to do!
        return;
    }

    if (active)
    {
        startSoundSystem();
    }
    else
    {
        destroySoundSystem();
    }
}

void AudioSystem::startSoundSystem()
{
    soundActive = true;
    soundThread = std::thread(&AudioSystem::soundThreadLoop, this);
}

void clear(std::queue<SoundSource>& q)
{
    // See: https://stackoverflow.com/a/709161/1624459
    std::queue<SoundSource> empty;
    std::swap(q, empty);
}

void AudioSystem::destroySoundSystem()
{
    soundActive = false;
    stopAllSounds();
    clear(soundQueue);
    soundReadyCondition.notify_one();
    soundThread.join();
}

void AudioSystem::stopAllSounds()
{
    // TODO
}

void AudioSystem::playSound(const AudioStore& audioStore, int soundId, SoundConfig cfg)
{
    std::shared_ptr<const WaveFile> waveFile = audioStore.getSound(soundId);
    if (!waveFile)
    {
        LOG_WARN("Tried to play invalid sound: {}", soundId);
        return;
    }

    SoundSource soundSource = { waveFile, cfg };
    playSound(soundSource);
}

void AudioSystem::playSound(SoundSource source)
{
    // Periodically clean up sounds that have finished
    cleanUpPlayedSounds();

    const std::scoped_lock<std::mutex> lock(soundQueueMutex);

    // Add the sound to the queue
    soundQueue.push(source);

    // Notify our sound thread of the new sound
    soundReadyCondition.notify_one();
}

void AudioSystem::cleanUpPlayedSounds()
{
    for (auto it = playedSounds.cbegin(); it != playedSounds.cend(); /* no increment */)
    {
        ALuint sourceId = it->first;
        if (isSoundPlaying(sourceId))
        {
            ++it;
        }
        else
        {
            alDeleteSources(1, &sourceId);
            it = playedSounds.erase(it);
        }
    }
}

bool AudioSystem::isSoundPlaying(ALuint sourceId) const
{
    ALint state;
    alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

}  // namespace Rival
