#include "catch2/catch.h"

#include <memory>

#include "entity/Entity.h"
#include "entity/components/SpriteComponent.h"
#include "entity/components/UnitAnimationComponent.h"
#include "game/Animations.h"
#include "game/UnitDef.h"
#include "gfx/Spritesheet.h"
#include "gfx/Texture.h"
#include "utils/TimeUtils.h"

using namespace Rival;

namespace TestAnimationComponent {

json defaultUnitDefJson = R"(
    {
        "name": "Test Unit",
        "portrait": 0,
        "animations":
        {
            "standing":
            {
                "startIndex": 3,
                "endIndex": 5
            }
        },
        "sounds": {}
    }
)"_json;

SCENARIO("AnimationComponent sets the txIndex for a single-frame animation", "[components][animation-component]")
{
    std::shared_ptr<Texture> tex = std::make_shared<Texture>(0, 128, 128);
    const Spritesheet spritesheet(tex, 16, 16, 0);

    GIVEN("An entity with an AnimationComponent, with a single-frame animation")
    {
        /* clang-format off */
        json unitDefJson = defaultUnitDefJson;
        unitDefJson["animations"]["standing"] = {
            { "startIndex", 3 },
            { "endIndex", 5 }
        };
        /* clang-format on */

        Unit e(UnitType::Knight, "", false);
        UnitDef unitDef = UnitDef::fromJson(unitDefJson);

        e.attach(std::make_shared<SpriteComponent>(spritesheet));
        e.attach(std::make_shared<UnitAnimationComponent>(unitDef));

        const SpriteComponent* spriteComponent = e.requireComponent<SpriteComponent>(SpriteComponent::key);
        UnitAnimationComponent* animComponent = e.requireComponent<UnitAnimationComponent>(UnitAnimationComponent::key);

        REQUIRE(spriteComponent->getTxIndex() == 0);

        WHEN("the entity is spawned")
        {
            e.addedToWorld(nullptr, 0, { 0, 0 });

            THEN("the entity's SpriteComponent has its txIndex set")
            {
                REQUIRE(spriteComponent->getTxIndex() == 3);
            }

            AND_WHEN("the AnimationComponent is updated")
            {
                animComponent->update();

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
        /* clang-format off */
        json unitDefJson = defaultUnitDefJson;
        unitDefJson["animations"]["standing"] = {
            { "startIndex", 3 },
            { "endIndex", 5 },
            { "msPerFrame", static_cast<int>(1.5f * TimeUtils::timeStepMs) }
        };
        /* clang-format on */

        Unit e(UnitType::Knight, "", false);
        UnitDef unitDef = UnitDef::fromJson(unitDefJson);

        e.attach(std::make_shared<SpriteComponent>(spritesheet));
        e.attach(std::make_shared<UnitAnimationComponent>(unitDef));
        e.addedToWorld(nullptr, 0, { 0, 0 });

        const SpriteComponent* spriteComponent = e.requireComponent<SpriteComponent>(SpriteComponent::key);
        UnitAnimationComponent* animComponent = e.requireComponent<UnitAnimationComponent>(UnitAnimationComponent::key);

        REQUIRE(spriteComponent->getTxIndex() == 3);

        WHEN("the AnimationComponent is updated once")
        {
            animComponent->update();

            THEN("the txIndex of the entity's SpriteComponent is unchanged")
            {
                // 1 * timeStepMs elapsed
                REQUIRE(spriteComponent->getTxIndex() == 3);
            }
        }

        WHEN("the AnimationComponent is updated twice")
        {
            animComponent->update();
            animComponent->update();

            THEN("the txIndex of the entity's SpriteComponent has increased by 1")
            {
                // 2 * timeStepMs elapsed
                REQUIRE(spriteComponent->getTxIndex() == 4);
            }
        }

        WHEN("the AnimationComponent is updated three times")
        {
            animComponent->update();
            animComponent->update();
            animComponent->update();

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
        /* clang-format off */
        json unitDefJson = defaultUnitDefJson;
        unitDefJson["animations"]["standing"] = {
            { "startIndex", 3 },
            { "endIndex", 4 },
            { "msPerFrame", static_cast<int>(TimeUtils::timeStepMs) }
        };
        /* clang-format on */

        Unit e(UnitType::Knight, "", false);
        UnitDef unitDef = UnitDef::fromJson(unitDefJson);

        e.attach(std::make_shared<SpriteComponent>(spritesheet));
        e.attach(std::make_shared<UnitAnimationComponent>(unitDef));
        e.addedToWorld(nullptr, 0, { 0, 0 });

        const SpriteComponent* spriteComponent = e.requireComponent<SpriteComponent>(SpriteComponent::key);
        UnitAnimationComponent* animComponent = e.requireComponent<UnitAnimationComponent>(UnitAnimationComponent::key);

        REQUIRE(spriteComponent->getTxIndex() == 3);

        WHEN("the AnimationComponent is updated once")
        {
            animComponent->update();

            THEN("the txIndex of the entity's SpriteComponent is increased by 1")
            {
                REQUIRE(spriteComponent->getTxIndex() == 4);
            }
        }

        WHEN("the AnimationComponent is updated twice")
        {
            animComponent->update();
            animComponent->update();

            THEN("the txIndex of the entity's SpriteComponent is back to the initial value")
            {
                REQUIRE(spriteComponent->getTxIndex() == 3);
            }
        }
    }
}

}  // namespace TestAnimationComponent
