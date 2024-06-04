#include <memory>
#include <unordered_map>

#include "application/Resources.h"
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

// TMP: Eventually we should create a MockResources object
class MockAudioStore final : public AudioStore
{
    std::shared_ptr<const WaveFile> getSound(int id) const override
    {
        return {};
    };

    std::shared_ptr<const MidiFile> getMidi(int id) const override
    {
        return {};
    }
};

std::shared_ptr<Unit> makeUnit();

std::shared_ptr<Unit> makeUnit(const UnitConfig& cfg);

}  // namespace EntityTestUtils
