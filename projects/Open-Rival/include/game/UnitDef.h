#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "audio/Sounds.h"
#include "game/Animations.h"
#include "utils/JsonUtils.h"

namespace Rival {

enum class MovementMode : std::uint8_t
{
    Walking,
    Seafaring,
    Flying
};

class UnitDef
{
public:
    static UnitDef fromJson(const json& j);

    UnitDef(std::string name,
            int portraitId,
            MovementMode movementMode,
            std::unordered_map<UnitAnimationType, const Animation> animations,
            std::unordered_map<UnitSoundType, const SoundBank> soundBanks);

    const Animation* getAnimation(UnitAnimationType animType) const;

    const SoundBank* getSoundBank(UnitSoundType soundType) const;

private:
    static void tryReadAnimation(const json& rawAnims,
            const std::string& key,
            UnitAnimationType animType,
            std::unordered_map<UnitAnimationType, const Animation>& animations);

    static void tryReadSoundBank(const json& rawSounds,
            const std::string& key,
            UnitSoundType soundType,
            std::unordered_map<UnitSoundType, const SoundBank>& soundBanks);

    static MovementMode getMovementMode(const std::string& s);

public:
    /** The default name for this unit type. */
    std::string name;

    /** ID of this unit's portrait image. */
    int portraitId = 0;

    /** How this unit moves around the map. */
    MovementMode movementMode;

private:
    /**
     * Animations by type.
     *
     * These specify the animation frames when the unit is facing south, and other facings are derived from them.
     */
    std::unordered_map<UnitAnimationType, const Animation> animations;

    /** Sound banks by type. */
    std::unordered_map<UnitSoundType, const SoundBank> soundBanks;
};

}  // namespace Rival
