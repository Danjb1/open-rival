#include "pch.h"
#include "AudioUtils.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include <stdexcept>

namespace Rival {
namespace AudioUtils {

    ALCdevice* openALDevice;

    void initAL() {
        // Find the default audio device
        openALDevice = alcOpenDevice(nullptr);
        if (!openALDevice) {
            throw std::runtime_error("Failed to find audio device");
        }
    }

    void destroyAL() {
        ALCboolean closed;
        closed = alcCloseDevice(openALDevice);
        if (!closed) {
            // TODO: log a warning
        }
    }

    /**
     * Based on:
     * https://indiegamedev.net/2020/02/15/the-complete-guide-to-openal-with-c-part-1-playing-a-sound/
     */
    bool checkALErrors() {
        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            switch (error) {
            case AL_INVALID_NAME:
                std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
                break;
            case AL_INVALID_ENUM:
                std::cerr << "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
                break;
            case AL_INVALID_VALUE:
                std::cerr << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                break;
            case AL_INVALID_OPERATION:
                std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
                break;
            case AL_OUT_OF_MEMORY:
                std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
                break;
            default:
                std::cerr << "UNKNOWN AL ERROR: " << error;
            }
            std::cerr << std::endl;
            return false;
        }
        return true;
    }

    bool checkALCErrors(ALCdevice* device) {
        ALCenum error = alcGetError(device);
        if (error != ALC_NO_ERROR) {
            switch (error) {
            case ALC_INVALID_VALUE:
                std::cerr << "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                break;
            case ALC_INVALID_DEVICE:
                std::cerr << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
                break;
            case ALC_INVALID_CONTEXT:
                std::cerr << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
                break;
            case ALC_INVALID_ENUM:
                std::cerr << "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
                break;
            case ALC_OUT_OF_MEMORY:
                std::cerr << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
                break;
            default:
                std::cerr << "UNKNOWN ALC ERROR: " << error;
            }
            std::cerr << std::endl;
            return false;
        }
        return true;
    }

}}  // namespace Rival::AudioUtils
