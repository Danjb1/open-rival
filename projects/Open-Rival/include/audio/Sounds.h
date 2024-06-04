#pragma once

#include <random>
#include <vector>

#include "game/UnitType.h"

namespace Rival {

enum class UnitSoundType : std::uint8_t
{
    Train,
    Select,
    Move
};

enum class BuildingSoundType : std::uint8_t
{
    Select
};

/** Settings used to play a sound.
 * By default, a sound will play at the listener's location. */
struct SoundConfig
{
    float pitch { 1.0f };
    float gain { 1.0f };
    std::vector<float> position { 0, 0, 0 };
    std::vector<float> velocity { 0, 0, 0 };
    bool looping { false };
    bool positionRelativeToListener { true };

    /** Unit type that generated the sound, if applicable.
     * This is used to limit how many voices we play. */
    UnitType unitType = UnitType::Invalid;
};

class SoundBank
{
public:
    SoundBank(const std::vector<int> sounds);

    int getRandomSound() const;

private:
    static std::mt19937 rng;

    const std::vector<int> sounds;
};

}  // namespace Rival
