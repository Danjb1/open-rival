#include "utils/EntityTestUtils.h"

#include "application/Resources.h"
#include "audio/AudioSystem.h"
#include "entity/EntityFactory.h"
#include "entity/components/AttackComponent.h"
#include "entity/components/FacingComponent.h"
#include "entity/components/HealthComponent.h"
#include "entity/components/OwnerComponent.h"
#include "entity/components/PassabilityComponent.h"
#include "entity/components/SpriteComponent.h"
#include "entity/components/UnitAnimationComponent.h"
#include "entity/components/WalkerComponent.h"
#include "gfx/Spritesheet.h"
#include "gfx/Texture.h"

namespace EntityTestUtils {

UnitConfig makeDefaultUnitConfig()
{
    UnitConfig cfg;
    cfg.animations.insert({ UnitAnimationType::Attacking, Animation(0, 3) });
    return cfg;
}
UnitConfig defaultUnitConfig = makeDefaultUnitConfig();

const Animation* UnitConfig::getAnimation(UnitAnimationType animType) const
{
    const auto iter = animations.find(animType);
    return iter == animations.cend() ? nullptr : &iter->second;
}

std::shared_ptr<Unit> makeUnit()
{
    return makeUnit(defaultUnitConfig);
}

std::shared_ptr<Unit> makeUnit(const UnitConfig& cfg)
{
    // Horrible hacks - these should live in some kind of TestResources or TestContext object
    // TODO: better still, just use the regular EntityFactory, don't try to be clever
    static Resources res;
    static std::shared_ptr<Texture> texture = std::make_shared<Texture>(0, 64, 64);
    static std::shared_ptr<Spritesheet> spritesheet = std::make_shared<Spritesheet>(texture, 32, 32, 0);
    static AudioSystem mockAudioSystem;
    static MockAudioStore mockAudioStore;
    static std::shared_ptr<std::mt19937> randomizer;
    static std::shared_ptr<EntityFactory> entityFactory =
            std::make_shared<EntityFactory>(res, mockAudioSystem, randomizer);

    auto unit = std::make_shared<Unit>(UnitType::Knight, "", false, 0.f);
    unit->attach(std::make_shared<HealthComponent>(100));
    unit->attach(std::make_shared<OwnerComponent>(0));
    unit->attach(std::make_shared<FacingComponent>(Facing::South));
    unit->attach(std::make_shared<SpriteComponent>(spritesheet, 0.f));
    unit->attach(std::make_shared<UnitAnimationComponent>(cfg));
    unit->attach(std::make_shared<WalkerComponent>());
    unit->attach(std::make_shared<PassabilityComponent>(TilePassability::GroundUnit));
    unit->attach(std::make_shared<AttackComponent>(mockAudioStore, res, mockAudioSystem, entityFactory, randomizer));

    return unit;
}

}  // namespace EntityTestUtils
