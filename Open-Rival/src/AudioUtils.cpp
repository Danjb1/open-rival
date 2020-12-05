#include "pch.h"
#include "AudioUtils.h"

#include <AL/al.h>
#include <iostream>
#include <stdexcept>

namespace Rival {
namespace AudioUtils {

    ALCdevice* openALDevice;
    ALCcontext* openALContext;

    void initAL() {
        // Find the default audio device
        openALDevice = alcOpenDevice(nullptr);
        if (!openALDevice) {
            throw std::runtime_error("Failed to find audio device");
        }

        // Create the OpenAL context
        openALContext = alcCreateContext(openALDevice, nullptr);
        if (!openALContext) {
            throw std::runtime_error("Failed to create audio context");
        }

        // Make this context current
        if (alcMakeContextCurrent(openALContext) != ALC_TRUE) {
            throw std::runtime_error("Failed to make audio context current");
        }
    }

    void destroyAL() {
        // Clear active audio context
        if (alcMakeContextCurrent(nullptr) != ALC_TRUE) {
            std::cerr << "Failed to clear active audio context\n";
        }

        // Destroy audio context
        alcDestroyContext(openALContext);
        if (alcGetError(openALDevice) != ALC_NO_ERROR) {
            std::cerr << "Failed to destroy audio context\n";
        }

        // Close audio device
        if (alcCloseDevice(openALDevice) != ALC_TRUE) {
            std::cerr << "Failed to close audio device\n";
        }
    }

    void checkPlaySoundALError(const WaveFile& waveFile) {
        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            std::cerr << "OpenAL error: " << error << "\n";
            throw std::runtime_error(
                    "Error playing sound: " + waveFile.filename);
        }
    }

    ALenum getAudioFormat(const WavHeader& waveHeader) {
        if (waveHeader.numChannels == 1 && waveHeader.bitsPerSample == 8) {
            return AL_FORMAT_MONO8;
        } else if (waveHeader.numChannels == 1 && waveHeader.bitsPerSample == 16) {
            return AL_FORMAT_MONO16;
        } else if (waveHeader.numChannels == 2 && waveHeader.bitsPerSample == 8) {
            return AL_FORMAT_STEREO8;
        } else if (waveHeader.numChannels == 2 && waveHeader.bitsPerSample == 16) {
            return AL_FORMAT_STEREO16;
        }
        throw std::runtime_error("Unrecognised wave format: "
                + std::to_string(waveHeader.numChannels) + " channels, "
                + std::to_string(waveHeader.bitsPerSample) + " bps");
    }

    void playSound(const WaveFile& waveFile) {
        // Generate buffer
        ALuint buffer;
        alGenBuffers(1, &buffer);
        checkPlaySoundALError(waveFile);

        // Fill buffer
        ALenum format = getAudioFormat(waveFile.header);
        alBufferData(
                buffer,
                format,
                waveFile.soundData.data(),
                waveFile.soundData.size(),
                waveFile.header.samplesPerSec);
        checkPlaySoundALError(waveFile);

        // Create sound source
        ALuint source;
        alGenSources(1, &source);
        checkPlaySoundALError(waveFile);
        alSourcef(source, AL_PITCH, 1);
        alSourcef(source, AL_GAIN, 1.0f);
        alSource3f(source, AL_POSITION, 0, 0, 0);
        alSource3f(source, AL_VELOCITY, 0, 0, 0);
        alSourcei(source, AL_LOOPING, AL_FALSE);
        alSourcei(source, AL_BUFFER, buffer);
        checkPlaySoundALError(waveFile);

        // Start playing sound
        alSourcePlay(source);
        checkPlaySoundALError(waveFile);

        // Play sound until done
        ALint state = AL_PLAYING;
        while (state == AL_PLAYING) {
            alGetSourcei(source, AL_SOURCE_STATE, &state);
            checkPlaySoundALError(waveFile);
        }

        // Delete sound source
        alDeleteSources(1, &source);
        checkPlaySoundALError(waveFile);

        // Delete buffer
        alDeleteBuffers(1, &buffer);
        checkPlaySoundALError(waveFile);
    }

}}  // namespace Rival::AudioUtils
