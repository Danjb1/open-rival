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

class UnitDef final : public AnimationContainer<UnitAnimationType>
{
public:
    static UnitDef fromJson(const json& j);

    // Begin AnimationContainer override
    const Animation* getAnimation(UnitAnimationType animType) const override;
    // End AnimationContainer override

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

    UnitDef() {}

public:
    /** The default name for this unit type. */
    std::string name;

    /** The attack ID of this unit's default attack. */
    int attackId = -1;

    /** ID of this unit's portrait image. */
    int portraitId = 0;

    /** How this unit moves around the map. */
    MovementMode movementMode = MovementMode::Walking;

    /** Name of the effect to spawn on death. */
    std::string deathEffectName;

    /** Size of the hitbox, in pixels. */
    int hitboxWidth = 0;
    int hitboxHeight = 0;

    /** Y-offset (pixels) applied to the unit's hitbox, health bar and death effect. */
    float offsetY = 0.f;

    /** Whether this unit is a vehicle (no inventory, cannot heal, does not rotate when idle). */
    bool isVehicle = false;

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
