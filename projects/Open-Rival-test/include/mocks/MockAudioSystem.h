#pragma once

#include "audio/AudioSystem.h"

namespace Rival {

/** AudioSystem that does nothing, so that tests never need an audio device. */
class MockAudioSystem : public AudioSystem
{
public:
    void setMidiActive(bool) override {}

    void playMidi(std::shared_ptr<const MidiFile>) override {}

    void setSoundActive(bool) override {}

    void prepareSounds(std::vector<std::shared_ptr<const WaveFile>>) override {}

    void playSound(const AudioStore&, int, SoundConfig) override {}

    void playSound(SoundSource) override {}

    bool isSoundPlaying(SoundSourceId) const override
    {
        return false;
    }

    std::unordered_map<SoundSourceId, SoundSource> getPlayedSounds() const override
    {
        return {};
    }
};

}  // namespace Rival
