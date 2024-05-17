#pragma once

#include <memory>
#include <string>

#include "audio/Sounds.h"
#include "entity/components/EntityComponent.h"
#include "game/UnitType.h"

namespace Rival {

class AudioStore;
class AudioSystem;
class UnitDef;

/**
 * Component that allows a unit to play voice clips.
 */
class VoiceComponent : public EntityComponent
{

public:
    VoiceComponent(const AudioStore& audioStore, AudioSystem& audioSystem, const UnitDef& unitDef);

    void playSound(UnitSoundType soundType);

private:
    bool isUnitTypeAlreadySpeaking(UnitType unitType) const;

public:
    static const std::string key;

private:
    const AudioStore& audioStore;

    AudioSystem& audioSystem;

    const UnitDef& unitDef;
};

}  // namespace Rival
