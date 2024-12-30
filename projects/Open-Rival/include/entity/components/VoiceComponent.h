#pragma once

#include <memory>
#include <string>

#include "audio/Sounds.h"
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
    , public HealthListener
{

public:
    VoiceComponent(const AudioStore& audioStore, AudioSystem& audioSystem, const UnitDef& unitDef);

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    void destroy() override;
    // End EntityComponent override

    // Begin HealthListener override
    void onHealthChanged(int prevValue, int newValue) override;
    void onMaxHealthChanged(int prevValue, int newValue) override;
    void onHealthDepleted() override;
    // Begin HealthListener override

    void playSound(UnitSoundType soundType);

private:
    bool isUnitTypeAlreadySpeaking(UnitType unitType) const;

public:
    static const std::string key;

private:
    const AudioStore& audioStore;
    AudioSystem& audioSystem;

    std::weak_ptr<HealthComponent> weakHealthComponent;

    const UnitDef& unitDef;
};

}  // namespace Rival
