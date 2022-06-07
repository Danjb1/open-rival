#include "pch.h"
#include "catch2/catch.h"

#include <memory>

#include "AnimationComponent.h"
#include "Animations.h"
#include "Entity.h"
#include "SpriteComponent.h"
#include "Spritesheet.h"
#include "Texture.h"
#include "TimerUtils.h"

using namespace Rival;

SCENARIO("AnimationComponent sets the txIndex for a single-frame animation", "[components][animation-component]")
{
    const Texture tex(0, 128, 128);
    const Spritesheet spritesheet(tex, 16, 16);

    GIVEN("An entity with an AnimationComponent, with a single-frame animation")
    {
        Entity e(1, 1);
        e.attach(std::make_unique<SpriteComponent>(spritesheet));
        e.attach(std::make_unique<AnimationComponent>(Animations::Animation { 3, 3, TimerUtils::timeStepMs }));

        const SpriteComponent* spriteComponent = e.requireComponent<SpriteComponent>(SpriteComponent::key);
        AnimationComponent* animComponent = e.requireComponent<AnimationComponent>(AnimationComponent::key);

        REQUIRE(spriteComponent->getTxIndex() == 0);

        WHEN("the entity is spawned")
        {
            e.onSpawn(nullptr, 0, { 0, 0 });

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
    const Texture tex(0, 128, 128);
    const Spritesheet spritesheet(tex, 16, 16);

    GIVEN("An entity with an AnimationComponent, where each frame of the animation lasts 1.5 * the time step")
    {
        Entity e(1, 1);
        e.attach(std::make_unique<SpriteComponent>(spritesheet));
        int msPerFrame = static_cast<int>(1.5f * TimerUtils::timeStepMs);
        e.attach(std::make_unique<AnimationComponent>(Animations::Animation { 3, 5, msPerFrame }));
        e.onSpawn(nullptr, 0, { 0, 0 });

        const SpriteComponent* spriteComponent = e.requireComponent<SpriteComponent>(SpriteComponent::key);
        AnimationComponent* animComponent = e.requireComponent<AnimationComponent>(AnimationComponent::key);

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
    const Texture tex(0, 128, 128);
    const Spritesheet spritesheet(tex, 16, 16);

    GIVEN("An entity with an AnimationComponent, and a 2-frame animation")
    {
        Entity e(1, 1);
        e.attach(std::make_unique<SpriteComponent>(spritesheet));
        e.attach(std::make_unique<AnimationComponent>(Animations::Animation { 3, 4, TimerUtils::timeStepMs }));
        e.onSpawn(nullptr, 0, { 0, 0 });

        const SpriteComponent* spriteComponent = e.requireComponent<SpriteComponent>(SpriteComponent::key);
        AnimationComponent* animComponent = e.requireComponent<AnimationComponent>(AnimationComponent::key);

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
