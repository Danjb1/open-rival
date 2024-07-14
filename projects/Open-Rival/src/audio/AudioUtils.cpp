#include "audio/AudioUtils.h"

#include <AL/alc.h>

#include <memory>
#include <stdexcept>

#include "audio/Sounds.h"
#include "utils/LogUtils.h"

namespace Rival { namespace AudioUtils {

ALCdevice* openALDevice;
ALCcontext* openALContext;

void initAL()
{
    // Find the default audio device
    openALDevice = alcOpenDevice(nullptr);
    if (!openALDevice)
    {
        throw std::runtime_error("Failed to find audio device");
    }

    // Create the OpenAL context
    openALContext = alcCreateContext(openALDevice, nullptr);
    if (!openALContext)
    {
        throw std::runtime_error("Failed to create audio context");
    }

    // Make this context current
    if (alcMakeContextCurrent(openALContext) != ALC_TRUE)
    {
        throw std::runtime_error("Failed to make audio context current");
    }
}

void destroyAL()
{
    // Clear active audio context
    if (alcMakeContextCurrent(nullptr) != ALC_TRUE)
    {
        LOG_WARN("Failed to clear active audio context");
    }

    // Destroy audio context
    alcDestroyContext(openALContext);
    if (alcGetError(openALDevice) != ALC_NO_ERROR)
    {
        LOG_WARN("Failed to destroy audio context");
    }

    // Close audio device
    if (alcCloseDevice(openALDevice) != ALC_TRUE)
    {
        LOG_WARN("Failed to close audio device");
    }
}

void checkPlaySoundALError(std::shared_ptr<const WaveFile> waveFile)
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        LOG_ERROR("OpenAL error: {}", error);
        throw std::runtime_error("Error playing sound: " + waveFile->filename);
    }
}

void checkOpenALError()
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        throw std::runtime_error("OpenAL error: " + std::to_string(error));
    }
}

ALenum getAudioFormat(const WavHeader& waveHeader)
{
    if (waveHeader.numChannels == 1 && waveHeader.bitsPerSample == 8)
    {
        return AL_FORMAT_MONO8;
    }
    else if (waveHeader.numChannels == 1 && waveHeader.bitsPerSample == 16)
    {
        return AL_FORMAT_MONO16;
    }
    else if (waveHeader.numChannels == 2 && waveHeader.bitsPerSample == 8)
    {
        return AL_FORMAT_STEREO8;
    }
    else if (waveHeader.numChannels == 2 && waveHeader.bitsPerSample == 16)
    {
        return AL_FORMAT_STEREO16;
    }
    throw std::runtime_error("Unrecognised wave format: " + std::to_string(waveHeader.numChannels) + " channels, "
            + std::to_string(waveHeader.bitsPerSample) + " bps");
}

ALuint createBuffer(std::shared_ptr<const WaveFile> waveFile)
{
    // Generate buffer
    ALuint buffer;
    alGenBuffers(1, &buffer);
    checkOpenALError();

    // Fill buffer
    ALenum format = getAudioFormat(waveFile->header);
    alBufferData(buffer,
            format,
            waveFile->soundData.data(),
            static_cast<ALsizei>(waveFile->soundData.size()),
            waveFile->header.samplesPerSec);
    checkOpenALError();

    return buffer;
}

void destroyBuffer(ALuint buffer)
{
    alDeleteBuffers(1, &buffer);
    checkOpenALError();
}

ALuint playSound(const SoundSource& source, ALuint buffer)
{
    // Create sound source
    const SoundConfig& cfg = source.cfg;
    ALuint sourceId;
    alGenSources(1, &sourceId);
    checkPlaySoundALError(source.waveFile);
    alSourcef(sourceId, AL_PITCH, cfg.pitch);
    alSourcef(sourceId, AL_GAIN, cfg.gain);
    alSource3f(sourceId, AL_POSITION, cfg.position[0], cfg.position[1], cfg.position[2]);
    alSourcei(sourceId, AL_SOURCE_RELATIVE, cfg.positionRelativeToListener ? AL_TRUE : AL_FALSE);
    alSource3f(sourceId, AL_VELOCITY, cfg.velocity[0], cfg.velocity[1], cfg.velocity[2]);
    alSourcei(sourceId, AL_LOOPING, cfg.looping ? AL_TRUE : AL_FALSE);
    alSourcei(sourceId, AL_BUFFER, buffer);
    checkPlaySoundALError(source.waveFile);

    // Start playing sound
    alSourcePlay(sourceId);
    checkPlaySoundALError(source.waveFile);

    return sourceId;
}

}}  // namespace Rival::AudioUtils
