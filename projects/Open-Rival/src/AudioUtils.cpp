#include "AudioUtils.h"

#include <AL/alc.h>

#include <iostream>
#include <stdexcept>

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
        std::cerr << "Failed to clear active audio context\n";
    }

    // Destroy audio context
    alcDestroyContext(openALContext);
    if (alcGetError(openALDevice) != ALC_NO_ERROR)
    {
        std::cerr << "Failed to destroy audio context\n";
    }

    // Close audio device
    if (alcCloseDevice(openALDevice) != ALC_TRUE)
    {
        std::cerr << "Failed to close audio device\n";
    }
}

void checkPlaySoundALError(const WaveFile& waveFile)
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        std::cerr << "OpenAL error: " << error << "\n";
        throw std::runtime_error("Error playing sound: " + waveFile.filename);
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
    throw std::runtime_error(
            "Unrecognised wave format: " + std::to_string(waveHeader.numChannels) + " channels, "
            + std::to_string(waveHeader.bitsPerSample) + " bps");
}

ALuint playSound(const SoundSource& source)
{
    // Generate buffer
    ALuint buffer;
    alGenBuffers(1, &buffer);
    checkPlaySoundALError(source.waveFile);

    // Fill buffer
    ALenum format = getAudioFormat(source.waveFile.header);
    alBufferData(
            buffer,
            format,
            source.waveFile.soundData.data(),
            source.waveFile.soundData.size(),
            source.waveFile.header.samplesPerSec);
    checkPlaySoundALError(source.waveFile);

    // Create sound source
    ALuint sourceId;
    alGenSources(1, &sourceId);
    checkPlaySoundALError(source.waveFile);
    alSourcef(sourceId, AL_PITCH, source.pitch);
    alSourcef(sourceId, AL_GAIN, source.gain);
    alSource3f(sourceId, AL_POSITION, source.position[0], source.position[1], source.position[2]);
    alSourcei(sourceId, AL_SOURCE_RELATIVE, source.positionRelativeToListener ? AL_TRUE : AL_FALSE);
    alSource3f(sourceId, AL_VELOCITY, source.velocity[0], source.velocity[1], source.velocity[2]);
    alSourcei(sourceId, AL_LOOPING, source.looping ? AL_TRUE : AL_FALSE);
    alSourcei(sourceId, AL_BUFFER, buffer);
    checkPlaySoundALError(source.waveFile);

    // Start playing sound
    alSourcePlay(sourceId);
    checkPlaySoundALError(source.waveFile);

    return sourceId;
}

}}  // namespace Rival::AudioUtils
