#pragma once

#include <memory>
#include <string>

#include "EntityComponent.h"
#include "Sounds.h"
#include "UnitPropsComponent.h"

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

    // Begin EntityComponent override
    void onEntitySpawned(World* world) override;
    // End EntityComponent override

    void playSound(UnitSoundType soundType);

private:
    bool isUnitTypeAlreadySpeaking() const;

public:
    static const std::string key;

private:
    const AudioStore& audioStore;

    AudioSystem& audioSystem;

    const UnitDef& unitDef;

    std::weak_ptr<UnitPropsComponent> weakUnitPropsComponent;
};

}  // namespace Rival
