#include "entity/components/VoiceComponent.h"

#include <AL/al.h>

#include "application/Resources.h"
#include "audio/AudioSystem.h"
#include "audio/SoundSource.h"
#include "audio/WaveFile.h"
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
    const WaveFile& waveFile = audioStore.getSound(soundId);
    SoundSource soundSource = { waveFile };
    soundSource.unitType = unit->getUnitType();

    audioSystem.playSound(soundSource);
}

bool VoiceComponent::isUnitTypeAlreadySpeaking(UnitType unitType) const
{
    // There can only be 1 voice clip playing at a time, per unit type
    std::unordered_map<ALuint, SoundSource> playedSounds = audioSystem.getPlayedSounds();

    for (const auto& playedSound : playedSounds)
    {
        const SoundSource& source = playedSound.second;
        if (source.unitType != unitType)
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

}  // namespace Rival
