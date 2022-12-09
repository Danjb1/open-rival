#include "pch.h"

#include "BuildingDef.h"

#include <tuple>
#include <utility>

namespace Rival {

BuildingDef BuildingDef::fromJson(const json& j)
{
    std::string name = j.at("name");
    int portraitId = j.at("portrait");

    const auto& rawAnims = j.at("animations");
    std::unordered_map<BuildingAnimationType, const Animation> animations;
    tryReadAnimation(rawAnims, "building", BuildingAnimationType::Building, animations);
    tryReadAnimation(rawAnims, "built", BuildingAnimationType::Built, animations);

    const auto& rawSounds = j.at("sounds");
    std::unordered_map<BuildingSoundType, const SoundBank> soundBanks;
    tryReadSoundBank(rawSounds, "select", BuildingSoundType::Select, soundBanks);

    return { name, portraitId, animations, soundBanks };
}

void BuildingDef::tryReadAnimation(
        const json& rawAnims,
        const std::string& key,
        BuildingAnimationType animType,
        std::unordered_map<BuildingAnimationType, const Animation>& animations)
{
    auto iter = rawAnims.find(key);
    if (iter == rawAnims.end())
    {
        // Animation not found
        return;
    }

    int startIndex = iter->at("startIndex");
    int endIndex = iter->at("endIndex");
    int msPerFrame = JsonUtils::getOrDefault(iter, "msPerFrame", Animation::defaultMsPerFrame);

    animations.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(animType),
            std::forward_as_tuple(startIndex, endIndex, msPerFrame, 0));
}

void BuildingDef::tryReadSoundBank(
        const json& rawSounds,
        const std::string& key,
        BuildingSoundType soundType,
        std::unordered_map<BuildingSoundType, const SoundBank>& soundBanks)
{
    auto iter = rawSounds.find(key);
    if (iter == rawSounds.end())
    {
        // SoundBank not found
        return;
    }

    soundBanks.emplace(soundType, *iter);
}

BuildingDef::BuildingDef(
        std::string name,
        int portraitId,
        std::unordered_map<BuildingAnimationType, const Animation> animations,
        std::unordered_map<BuildingSoundType, const SoundBank> soundBanks)
    : name(name)
    , portraitId(portraitId)
    , animations(animations)
    , soundBanks(soundBanks)
{
}

const Animation* BuildingDef::getAnimation(BuildingAnimationType animType) const
{
    auto iter = animations.find(animType);
    return iter == animations.cend() ? nullptr : &iter->second;
}

const SoundBank* BuildingDef::getSoundBank(BuildingSoundType soundType) const
{
    auto iter = soundBanks.find(soundType);
    return iter == soundBanks.cend() ? nullptr : &iter->second;
}

}  // namespace Rival
