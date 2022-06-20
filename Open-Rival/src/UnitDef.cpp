#include "pch.h"

#include "UnitDef.h"

#include <utility>

namespace Rival {

UnitDef UnitDef::fromJson(const json& j)
{
    std::string name = j.at("name");
    int portraitId = j.at("portrait");

    const auto& rawAnims = j.at("animations");
    std::unordered_map<UnitAnimationType, const Animation> animations;

    auto animIter = rawAnims.find("standing");
    if (animIter != rawAnims.end())
    {
        int startIndex = animIter->at("startIndex");
        int endIndex = animIter->at("endIndex");
        int msPerFrame = animIter->at("msPerFrame");
        const Animation anim = { startIndex, endIndex, msPerFrame };
        animations.emplace(UnitAnimationType::Standing, anim);
    }

    animIter = rawAnims.find("moving");
    if (animIter != rawAnims.end())
    {
        int startIndex = animIter->at("startIndex");
        int endIndex = animIter->at("endIndex");
        int msPerFrame = animIter->at("msPerFrame");
        const Animation anim = { startIndex, endIndex, msPerFrame };
        animations.emplace(UnitAnimationType::Moving, anim);
    }

    // rawAnims.at("attacking");

    const auto& rawSounds = j.at("sounds");
    std::unordered_map<UnitSoundType, const SoundBank> soundBanks;

    auto soundIter = rawSounds.find("select");
    if (soundIter != rawSounds.end())
    {
        const std::vector<int> soundIds = *soundIter;
        soundBanks.emplace(UnitSoundType::Select, soundIds);
    }

    // rawSounds.at("train");
    // rawSounds.at("move");

    return { name, portraitId, animations, soundBanks };
}

UnitDef::UnitDef(
        std::string name,
        int portraitId,
        std::unordered_map<UnitAnimationType, const Animation> animations,
        std::unordered_map<UnitSoundType, const SoundBank> soundBanks)
    : name(name)
    , portraitId(portraitId)
    , animations(animations)
    , soundBanks(soundBanks)
{
}

const Animation* UnitDef::getAnimation(UnitAnimationType animType) const
{
    auto iter = animations.find(animType);
    return iter == animations.cend() ? nullptr : &iter->second;
}

const SoundBank* UnitDef::getSoundBank(UnitSoundType soundType) const
{
    auto iter = soundBanks.find(soundType);
    return iter == soundBanks.cend() ? nullptr : &iter->second;
}

}  // namespace Rival
