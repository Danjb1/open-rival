#pragma once

#include <random>
#include <vector>

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
