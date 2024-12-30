#include "entity/components/VoiceComponent.h"

#include "application/Resources.h"
#include "audio/AudioSystem.h"
#include "entity/Entity.h"
#include "entity/Unit.h"
#include "game/UnitDef.h"

namespace Rival {

const std::string VoiceComponent::key = "voice";

VoiceComponent::VoiceComponent(const AudioStore& audioStore, AudioSystem& audioSystem, const UnitDef& unitDef)
    : EntityComponent(key)
    , audioStore(audioStore)
    , audioSystem(audioSystem)
    , unitDef(unitDef)
{
}

void VoiceComponent::onEntityFirstAddedToWorld(World*)
{
    weakHealthComponent = entity->requireComponentWeak<HealthComponent>(HealthComponent::key);
    if (auto healthComponent = weakHealthComponent.lock())
    {
        healthComponent->addListener(std::dynamic_pointer_cast<HealthListener>(shared_from_this()));
    }
}

void VoiceComponent::destroy()
{
    if (auto healthComponent = weakHealthComponent.lock())
    {
        healthComponent->removeListener(std::dynamic_pointer_cast<HealthListener>(shared_from_this()));
    }
}

void VoiceComponent::playSound(UnitSoundType soundType)
{
    const SoundBank* soundBank = unitDef.getSoundBank(soundType);
    if (!soundBank)
    {
        return;
    }

    Unit* unit = entity->as<Unit>();
    if (!unit)
    {
        // For now, we only support Unit voices; later we will need to support Buildings as well
        return;
    }

    if (isUnitTypeAlreadySpeaking(unit->getUnitType()))
    {
        return;
    }

    int soundId = soundBank->getRandomSound();
    SoundConfig soundCfg;
    soundCfg.unitType = unit->getUnitType();
    audioSystem.playSound(audioStore, soundId, soundCfg);
}

bool VoiceComponent::isUnitTypeAlreadySpeaking(UnitType unitType) const
{
    // There can only be 1 voice clip playing at a time, per unit type
    std::unordered_map<ALuint, SoundSource> playedSounds = audioSystem.getPlayedSounds();

    for (const auto& playedSound : playedSounds)
    {
        const SoundSource& source = playedSound.second;
        if (source.cfg.unitType != unitType)
        {
            continue;
        }

        ALuint sourceId = playedSound.first;
        if (audioSystem.isSoundPlaying(sourceId))
        {
            return true;
        }
    }

    // TODO: what about buildings?

    return false;
}

void VoiceComponent::onHealthChanged(int /*prevValue*/, int /*newValue*/)
{
    // Nothing to do
}

void VoiceComponent::onMaxHealthChanged(int /*prevValue*/, int /*newValue*/)
{
    // Nothing to do
}

void VoiceComponent::onHealthDepleted()
{
    Unit* unit = entity->as<Unit>();
    if (!unit)
    {
        return;
    }

    const SoundBank* soundBank = unitDef.getSoundBank(UnitSoundType::Die);
    if (!soundBank)
    {
        return;
    }

    int soundId = soundBank->getRandomSound();
    SoundConfig soundCfg;
    soundCfg.unitType = unit->getUnitType();
    audioSystem.playSound(audioStore, soundId, soundCfg);
}

}  // namespace Rival
