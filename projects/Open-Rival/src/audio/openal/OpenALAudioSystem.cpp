#include "audio/openal/OpenALAudioSystem.h"

#include <utility>

#include "application/Resources.h"
#include "audio/AudioUtils.h"
#include "audio/MidiPlayer.h"
#include "utils/LogUtils.h"

namespace Rival {

OpenALAudioSystem::OpenALAudioSystem()
    : midiActive(false)
    , soundActive(false)
{
    AudioUtils::initAL();
}

OpenALAudioSystem::~OpenALAudioSystem()
{
    setMidiActive(false);
    setSoundActive(false);
    destroyAllSounds();

    AudioUtils::destroyAL();
}

///////////////////////////////////////////////////////////////////////////
// MIDI playback
///////////////////////////////////////////////////////////////////////////

void OpenALAudioSystem::midiThreadLoop()
{
    while (midiActive)
    {
        waitForMidi();
        midiPlayer.play(currentMidiTrack);
    }
}

void OpenALAudioSystem::waitForMidi()
{
    std::unique_lock<std::mutex> midiReadyLock(midiMutex);
    while (midiActive && (!currentMidiTrack || currentMidiTrack->isEmpty()))
    {
        midiReadyCondition.wait(midiReadyLock);
    }
}

void OpenALAudioSystem::setMidiActive(bool active)
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

void OpenALAudioSystem::startMidiSystem()
{
    midiActive = true;
    midiPlayer.init();
    midiThread = std::thread(&OpenALAudioSystem::midiThreadLoop, this);
}

void OpenALAudioSystem::destroyMidiSystem()
{
    midiActive = false;
    midiPlayer.stop();
    midiReadyCondition.notify_one();
    midiThread.join();
}

void OpenALAudioSystem::playMidi(std::shared_ptr<const MidiFile> midi)
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

void OpenALAudioSystem::soundThreadLoop()
{
    while (soundActive)
    {
        waitForSound();
        playNextSound();
    }
}

void OpenALAudioSystem::waitForSound()
{
    std::unique_lock<std::mutex> soundReadyLock(soundQueueMutex);
    while (soundActive && soundQueue.size() == 0)
    {
        soundReadyCondition.wait(soundReadyLock);
    }
}

void OpenALAudioSystem::playNextSound()
{
    const std::scoped_lock<std::mutex> lock(soundQueueMutex);
    if (soundQueue.empty())
    {
        return;
    }

    SoundSource& sound = soundQueue.front();
    try
    {
        const auto foundBuffer = buffers.find(sound.waveFile);
        if (foundBuffer == buffers.cend())
        {
            LOG_WARN("Tried to play sound with no associated buffer: {}", sound.waveFile->filename);
            return;
        }
        ALuint bufferId = foundBuffer->second;
        ALint sourceId = AudioUtils::playSound(sound, bufferId);
        soundQueue.pop();
        playedSounds.emplace(sourceId, sound);
    }
    catch (const std::runtime_error& e)
    {
        // Don't crash the game because of an audio error
        LOG_ERROR(e.what());
    }
}

void OpenALAudioSystem::setSoundActive(bool active)
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

void OpenALAudioSystem::startSoundSystem()
{
    soundActive = true;
    soundThread = std::thread(&OpenALAudioSystem::soundThreadLoop, this);
}

void clear(std::queue<SoundSource>& q)
{
    // See: https://stackoverflow.com/a/709161/1624459
    std::queue<SoundSource> empty;
    std::swap(q, empty);
}

void OpenALAudioSystem::destroySoundSystem()
{
    soundActive = false;
    stopAllSounds();
    clear(soundQueue);
    soundReadyCondition.notify_one();
    soundThread.join();
}

void OpenALAudioSystem::stopAllSounds()
{
    for (const auto& sound : playedSounds)
    {
        ALuint sourceId = sound.first;
        alDeleteSources(1, &sourceId);
    }

    playedSounds.clear();
}

void OpenALAudioSystem::prepareSounds(std::vector<std::shared_ptr<const WaveFile>> sounds)
{
    for (const auto& sound : sounds)
    {
        ALuint bufferId = AudioUtils::createBuffer(sound);
        buffers.insert({ sound, bufferId });
    }
}

void OpenALAudioSystem::destroyAllSounds()
{
    for (const auto& buffer : buffers)
    {
        AudioUtils::destroyBuffer(buffer.second);
    }
}

void OpenALAudioSystem::playSound(const AudioStore& audioStore, int soundId, SoundConfig cfg)
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

void OpenALAudioSystem::playSound(SoundSource source)
{
    // Periodically clean up sounds that have finished
    cleanUpPlayedSounds();

    const std::scoped_lock<std::mutex> lock(soundQueueMutex);

    // Add the sound to the queue
    soundQueue.push(source);

    // Notify our sound thread of the new sound
    soundReadyCondition.notify_one();
}

void OpenALAudioSystem::cleanUpPlayedSounds()
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

bool OpenALAudioSystem::isSoundPlaying(SoundSourceId sourceId) const
{
    ALint state;
    alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

}  // namespace Rival
