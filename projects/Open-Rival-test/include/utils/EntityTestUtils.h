#include <memory>
#include <unordered_map>

#include "entity/Unit.h"
#include "entity/components/EntityComponent.h"
#include "game/Animations.h"
#include "game/UnitDef.h"

using namespace Rival;

namespace EntityTestUtils {

struct UnitConfig final : public AnimationContainer
{
    // Begin AnimationContainer override
    const Animation* getAnimation(UnitAnimationType animType) const override;
    // End AnimationContainer override

    std::unordered_map<UnitAnimationType, const Animation> animations = {};
};

std::shared_ptr<Unit> makeUnit();

std::shared_ptr<Unit> makeUnit(const UnitConfig& cfg);

}  // namespace EntityTestUtils
