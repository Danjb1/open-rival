#include "catch2/catch.h"

#include <memory>

#include "entity/Entity.h"
#include "entity/components/SpriteComponent.h"
#include "entity/components/UnitAnimationComponent.h"
#include "game/Animations.h"
#include "game/UnitDef.h"
#include "game/World.h"
#include "gfx/Spritesheet.h"
#include "gfx/Texture.h"
#include "utils/EntityTestUtils.h"
#include "utils/TimeUtils.h"

using namespace Rival;

namespace TestAnimationComponent {

SCENARIO("AnimationComponent sets the txIndex for a single-frame animation", "[components][animation-component]")
{
    std::shared_ptr<Texture> tex = std::make_shared<Texture>(0, 128, 128);
    const Spritesheet spritesheet(tex, 16, 16, 0);

    GIVEN("An entity with an AnimationComponent, with a single-frame animation")
    {
        EntityTestUtils::UnitConfig unitCfg;
        unitCfg.animations.insert({ UnitAnimationType::Standing, Animation(3, 3) });
        std::shared_ptr<Unit> e = EntityTestUtils::makeUnit(unitCfg);

        const SpriteComponent* spriteComponent = e->requireComponent<SpriteComponent>(SpriteComponent::key);
        UnitAnimationComponent* animComponent =
                e->requireComponent<UnitAnimationComponent>(UnitAnimationComponent::key);

        REQUIRE(spriteComponent->getTxIndex() == 0);

        WHEN("the entity is spawned")
        {
            World world(5, 5, false);
            e->addedToWorld(&world, 0, { 0, 0 });

            THEN("the entity's SpriteComponent has its txIndex set")
            {
                REQUIRE(spriteComponent->getTxIndex() == 3);
            }

            AND_WHEN("the AnimationComponent is updated until at least one animFrame has passed")
            {
                for (int t = 0; t < Animation::defaultMsPerFrame; ++t)
                {
                    animComponent->update(TimeUtils::timeStepMs);
                }

                THEN("the txIndex of the entity's SpriteComponent is unchanged")
                {
                    REQUIRE(spriteComponent->getTxIndex() == 3);
                }
            }
        }
    }
}

SCENARIO("AnimationComponent updates the txIndex at the right time", "[components][animation-component]")
{
    std::shared_ptr<Texture> tex = std::make_shared<Texture>(0, 128, 128);
    const Spritesheet spritesheet(tex, 16, 16, 0);

    GIVEN("An entity with an AnimationComponent, where each frame of the animation lasts 1.5 * the time step")
    {
        EntityTestUtils::UnitConfig unitCfg;
        unitCfg.animations.insert(
                { UnitAnimationType::Standing, Animation(3, 5, static_cast<int>(1.5f * TimeUtils::timeStepMs)) });
        std::shared_ptr<Unit> e = EntityTestUtils::makeUnit(unitCfg);

        World world(5, 5, false);
        e->addedToWorld(&world, 0, { 0, 0 });

        const SpriteComponent* spriteComponent = e->requireComponent<SpriteComponent>(SpriteComponent::key);
        UnitAnimationComponent* animComponent =
                e->requireComponent<UnitAnimationComponent>(UnitAnimationComponent::key);

        REQUIRE(spriteComponent->getTxIndex() == 3);

        WHEN("the AnimationComponent is updated once")
        {
            animComponent->update(TimeUtils::timeStepMs);

            THEN("the txIndex of the entity's SpriteComponent is unchanged")
            {
                // 1 * timeStepMs elapsed
                REQUIRE(spriteComponent->getTxIndex() == 3);
            }
        }

        WHEN("the AnimationComponent is updated twice")
        {
            animComponent->update(TimeUtils::timeStepMs);
            animComponent->update(TimeUtils::timeStepMs);

            THEN("the txIndex of the entity's SpriteComponent has increased by 1")
            {
                // 2 * timeStepMs elapsed
                REQUIRE(spriteComponent->getTxIndex() == 4);
            }
        }

        WHEN("the AnimationComponent is updated three times")
        {
            animComponent->update(TimeUtils::timeStepMs);
            animComponent->update(TimeUtils::timeStepMs);
            animComponent->update(TimeUtils::timeStepMs);

            THEN("the txIndex of the entity's SpriteComponent has increased by 2")
            {
                // 3 * timeStepMs elapsed
                REQUIRE(spriteComponent->getTxIndex() == 5);
            }
        }
    }
}

SCENARIO("AnimationComponent loops the animation back to the start", "[components][animation-component]")
{
    std::shared_ptr<Texture> tex = std::make_shared<Texture>(0, 128, 128);
    const Spritesheet spritesheet(tex, 16, 16, 0);

    GIVEN("An entity with an AnimationComponent, and a 2-frame animation")
    {
        EntityTestUtils::UnitConfig unitCfg;
        unitCfg.animations.insert(
                { UnitAnimationType::Standing, Animation(3, 4, static_cast<int>(TimeUtils::timeStepMs)) });
        std::shared_ptr<Unit> e = EntityTestUtils::makeUnit(unitCfg);

        World world(5, 5, false);
        e->addedToWorld(&world, 0, { 0, 0 });

        const SpriteComponent* spriteComponent = e->requireComponent<SpriteComponent>(SpriteComponent::key);
        UnitAnimationComponent* animComponent =
                e->requireComponent<UnitAnimationComponent>(UnitAnimationComponent::key);

        REQUIRE(spriteComponent->getTxIndex() == 3);

        WHEN("the AnimationComponent is updated once")
        {
            animComponent->update(TimeUtils::timeStepMs);

            THEN("the txIndex of the entity's SpriteComponent is increased by 1")
            {
                REQUIRE(spriteComponent->getTxIndex() == 4);
            }
        }

        WHEN("the AnimationComponent is updated twice")
        {
            animComponent->update(TimeUtils::timeStepMs);
            animComponent->update(TimeUtils::timeStepMs);

            THEN("the txIndex of the entity's SpriteComponent is back to the initial value")
            {
                REQUIRE(spriteComponent->getTxIndex() == 3);
            }
        }
    }
}

}  // namespace TestAnimationComponent
