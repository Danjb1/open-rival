#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Animations.h"
#include "JsonUtils.h"
#include "Sounds.h"

namespace Rival {

class BuildingDef
{
public:
    static BuildingDef fromJson(const json& j);

    BuildingDef(
            std::string name,
            int portraitId,
            std::unordered_map<BuildingAnimationType, const Animation> animations,
            std::unordered_map<BuildingSoundType, const SoundBank> soundBanks);

    const Animation* getAnimation(BuildingAnimationType animType) const;

    const SoundBank* getSoundBank(BuildingSoundType soundType) const;

private:
    static void tryReadAnimation(
            const json& rawAnims,
            const std::string& key,
            BuildingAnimationType animType,
            std::unordered_map<BuildingAnimationType, const Animation>& animations);

    static void tryReadSoundBank(
            const json& rawSounds,
            const std::string& key,
            BuildingSoundType soundType,
            std::unordered_map<BuildingSoundType, const SoundBank>& soundBanks);

public:
    /**
     * The default name for this building type.
     */
    std::string name;

    /**
     * ID of this building's portrait image.
     */
    int portraitId = 0;

private:
    /**
     * Animations by type.
     */
    std::unordered_map<BuildingAnimationType, const Animation> animations;

    /**
     * Sound banks by type.
     */
    std::unordered_map<BuildingSoundType, const SoundBank> soundBanks;
};

}  // namespace Rival
