#include "game/UnitDef.h"

#include <tuple>
#include <utility>

namespace Rival {

UnitDef UnitDef::fromJson(const json& j)
{
    UnitDef unitDef;

    // Basic properties
    unitDef.name = j.at("name");
    unitDef.attackId = j.value("attack", -1);
    unitDef.portraitId = j.at("portrait");
    std::string movementModeStr = j.value("movement", "ground");
    unitDef.movementMode = getMovementMode(movementModeStr);
    unitDef.deathEffectName = j.value("deathEffect", "");
    unitDef.offsetY = j.value("offsetY", 0.f);
    unitDef.isVehicle = j.value("vehicle", false);

    // Animations
    const auto& rawAnims = j.at("animations");
    tryReadAnimation(rawAnims, "standing", UnitAnimationType::Standing, unitDef.animations);
    tryReadAnimation(rawAnims, "standingWithBag", UnitAnimationType::StandingWithBag, unitDef.animations);
    tryReadAnimation(rawAnims, "moving", UnitAnimationType::Moving, unitDef.animations);
    tryReadAnimation(rawAnims, "movingWithBag", UnitAnimationType::MovingWithBag, unitDef.animations);
    tryReadAnimation(rawAnims, "attacking", UnitAnimationType::Attacking, unitDef.animations);
    tryReadAnimation(rawAnims, "harvesting", UnitAnimationType::Harvesting, unitDef.animations);
    tryReadAnimation(rawAnims, "dying", UnitAnimationType::Dying, unitDef.animations);

    // Sounds
    const auto& rawSounds = j.at("sounds");
    tryReadSoundBank(rawSounds, "select", UnitSoundType::Select, unitDef.soundBanks);
    tryReadSoundBank(rawSounds, "train", UnitSoundType::Train, unitDef.soundBanks);
    tryReadSoundBank(rawSounds, "move", UnitSoundType::Move, unitDef.soundBanks);
    tryReadSoundBank(rawSounds, "die", UnitSoundType::Die, unitDef.soundBanks);

    return unitDef;
}

void UnitDef::tryReadAnimation(const json& rawAnims,
        const std::string& key,
        UnitAnimationType animType,
        std::unordered_map<UnitAnimationType, const Animation>& animations)
{
    const auto iter = rawAnims.find(key);
    if (iter == rawAnims.end())
    {
        // Animation not found
        return;
    }

    const int startIndex = iter->at("startIndex");
    const int endIndex = iter->at("endIndex");
    const int msPerFrame = JsonUtils::getOrDefault(iter, "msPerFrame", Animation::defaultMsPerFrame);
    const int facingStride = JsonUtils::getOrDefault(iter, "facingStride", 0);

    animations.emplace(std::piecewise_construct,
            std::forward_as_tuple(animType),
            std::forward_as_tuple(startIndex, endIndex, msPerFrame, facingStride));
}

void UnitDef::tryReadSoundBank(const json& rawSounds,
        const std::string& key,
        UnitSoundType soundType,
        std::unordered_map<UnitSoundType, const SoundBank>& soundBanks)
{
    const auto iter = rawSounds.find(key);
    if (iter == rawSounds.end())
    {
        // SoundBank not found
        return;
    }

    soundBanks.emplace(soundType, *iter);
}

MovementMode UnitDef::getMovementMode(const std::string& s)
{
    if (s == "flying")
    {
        return MovementMode::Flying;
    }
    else if (s == "seafaring")
    {
        return MovementMode::Seafaring;
    }
    else
    {
        return MovementMode::Walking;
    }
}

const Animation* UnitDef::getAnimation(UnitAnimationType animType) const
{
    const auto iter = animations.find(animType);
    return iter == animations.cend() ? nullptr : &iter->second;
}

const SoundBank* UnitDef::getSoundBank(UnitSoundType soundType) const
{
    const auto iter = soundBanks.find(soundType);
    return iter == soundBanks.cend() ? nullptr : &iter->second;
}

}  // namespace Rival
