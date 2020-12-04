#include "pch.h"
#include "AudioUtils.h"

#include <AL/alc.h>
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

}}  // namespace Rival::AudioUtils
