#include "VoiceComponent.h"

#include <AL/al.h>

#include "AudioSystem.h"
#include "Entity.h"
#include "Resources.h"
#include "SoundSource.h"
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

void VoiceComponent::onEntitySpawned(World*)
{
    weakUnitPropsComponent = entity->getComponentWeak<UnitPropsComponent>(UnitPropsComponent::key);
}

void VoiceComponent::playSound(UnitSoundType soundType)
{
    const SoundBank* soundBank = unitDef.getSoundBank(soundType);
    if (!soundBank)
    {
        return;
    }

    if (isUnitTypeAlreadySpeaking())
    {
        return;
    }

    int soundId = soundBank->getRandomSound();
    const WaveFile& waveFile = audioStore.getSound(soundId);
    SoundSource soundSource = { waveFile };

    // Associate the sound source with our unit type
    if (auto unitPropsComponent = weakUnitPropsComponent.lock())
    {
        soundSource.unitType = unitPropsComponent->getUnitType();
    }

    audioSystem.playSound(soundSource);
}

bool VoiceComponent::isUnitTypeAlreadySpeaking() const
{
    // There can only be 1 voice clip playing at a time, per unit type
    if (auto unitPropsComponent = weakUnitPropsComponent.lock())
    {
        std::unordered_map<ALuint, SoundSource> playedSounds = audioSystem.getPlayedSounds();
        Unit::Type currentUnitType = unitPropsComponent->getUnitType();

        for (const auto& playedSound : playedSounds)
        {
            const SoundSource& source = playedSound.second;
            if (source.unitType != currentUnitType)
            {
                continue;
            }

            ALuint sourceId = playedSound.first;
            if (audioSystem.isSoundPlaying(sourceId))
            {
                return true;
            }
        }
    }

    // TODO: what about buildings?

    return false;
}

}  // namespace Rival
