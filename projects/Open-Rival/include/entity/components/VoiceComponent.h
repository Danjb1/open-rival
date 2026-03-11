#pragma once

#include <memory>
#include <string>

#include "audio/Sounds.h"
#include "entity/Unit.h"
#include "entity/components/EntityComponent.h"
#include "entity/components/HealthComponent.h"
#include "game/UnitType.h"

namespace Rival {

class AudioStore;
class AudioSystem;
class UnitDef;

/**
 * Component that allows a unit to play voice clips.
 */
class VoiceComponent
    : public EntityComponent
    , public UnitStateListener
{

public:
    VoiceComponent(const AudioStore& audioStore, AudioSystem& audioSystem, const UnitDef& unitDef);

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    void destroy() override;
    // End EntityComponent override

    // Begin UnitStateListener override
    void onUnitStateChanged(Unit* unit, const UnitState newState) override;
    // Begin UnitStateListener override

    void playSound(UnitSoundType soundType);

private:
    bool isUnitTypeAlreadySpeaking(UnitType unitType) const;
    void playDeathSound();

public:
    static const std::string key;

private:
    const AudioStore& audioStore;
    AudioSystem& audioSystem;

    std::weak_ptr<HealthComponent> weakHealthComponent;

    const UnitDef& unitDef;
};

}  // namespace Rival
