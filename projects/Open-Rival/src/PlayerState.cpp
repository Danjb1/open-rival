#include "pch.h"

#include "PlayerState.h"

namespace Rival {

PlayerState::PlayerState(Race race, int gold, int wood, int food)
    : race(race)
    , gold(gold)
    , wood(wood)
    , food(food)
{
}

}  // namespace Rival
