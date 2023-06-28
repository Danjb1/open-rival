#include "Sounds.h"

#include <cstdint>

namespace Rival {

std::mt19937 SoundBank::rng;

SoundBank::SoundBank(const std::vector<int> sounds)
    : sounds(sounds)
{
}

int SoundBank::getRandomSound() const
{
    if (sounds.empty())
    {
        return -1;
    }

    std::uniform_int_distribution<int> dist(0, sounds.size() - 1);
    int randIndex = dist(rng);

    return sounds[randIndex];
}

}  // namespace Rival
