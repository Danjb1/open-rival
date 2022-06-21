#include "pch.h"

#include "VoiceComponent.h"

#include "AudioSystem.h"
#include "Resources.h"
#include "UnitDef.h"
#include "WaveFile.h"

namespace Rival {

const std::string VoiceComponent::key = "voice";

VoiceComponent::VoiceComponent(const AudioStore& audioStore, AudioSystem& audioSystem, const UnitDef& unitDef)
    : EntityComponent(key)
    , audioStore(audioStore)
    , audioSystem(audioSystem)
    , unitDef(unitDef)
{
}

void VoiceComponent::playSound(UnitSoundType soundType)
{
    const SoundBank* soundBank = unitDef.getSoundBank(soundType);
    if (!soundBank)
    {
        return;
    }

    int soundId = soundBank->getRandomSound();
    const WaveFile& waveFile = audioStore.getSound(soundId);
    audioSystem.playSound({ waveFile });
}

}  // namespace Rival
