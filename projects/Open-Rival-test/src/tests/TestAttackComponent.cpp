#include <catch2/catch_test_macros.hpp>

#include <array>

#include "entity/Unit.h"
#include "entity/components/AttackComponent.h"
#include "entity/components/MovementComponent.h"
#include "game/Pathfinding.h"
#include "game/World.h"
#include "utils/EntityTestUtils.h"
#include "utils/TimeUtils.h"

using namespace Rival;

namespace TestAttackComponent {

SCENARIO("AttackComponent can initiate an attack", "[components][attack-component]")
{
    GIVEN("A unit with an AttackComponent")
    {
        World world(5, 5, false);

        std::shared_ptr<Unit> attacker = EntityTestUtils::makeUnit();
        world.addEntity(attacker, 1, 1);

        AttackComponent* attackComponent = attacker->requireComponent<AttackComponent>(AttackComponent::key);
        AttackDef attackDef;
        attackDef.range = 1;
        attackComponent->registerAttack(&attackDef);

        WHEN("targeting a unit in range")
        {
            std::shared_ptr<Unit> target = EntityTestUtils::makeUnit();
            world.addEntity(target, 2, 1);

            attackComponent->setTargetEntity(target);

            attacker->update(TimeUtils::timeStepMs);

            THEN("an attack is initiated")
            {
                REQUIRE(attacker->getState() == UnitState::Attacking);
            }
        }
    }
}

SCENARIO("AttackComponent ignores targets that no longer exist", "[components][attack-component]")
{
    GIVEN("A unit with an AttackComponent")
    {
        World world(5, 5, false);

        std::shared_ptr<Unit> attacker = EntityTestUtils::makeUnit();
        world.addEntity(attacker, 1, 1);

        AttackComponent* attackComponent = attacker->requireComponent<AttackComponent>(AttackComponent::key);
        AttackDef attackDef;
        attackDef.range = 1;
        attackComponent->registerAttack(&attackDef);

        WHEN("targeting a unit that has been destroyed")
        {
            std::weak_ptr<Entity> weakTarget;
            {
                std::shared_ptr<Unit> target = EntityTestUtils::makeUnit();
                weakTarget = target;
            }

            attackComponent->setTargetEntity(weakTarget);
            attacker->update(TimeUtils::timeStepMs);

            THEN("no attack is initiated")
            {
                REQUIRE(attacker->getState() == UnitState::Idle);
            }
        }
    }
}

SCENARIO("AttackComponent does nothing if it has no attacks", "[components][attack-component]")
{
    GIVEN("A unit with an AttackComponent but no registered attacks")
    {
        World world(5, 5, false);

        std::shared_ptr<Unit> attacker = EntityTestUtils::makeUnit();
        world.addEntity(attacker, 1, 1);

        AttackComponent* attackComponent = attacker->requireComponent<AttackComponent>(AttackComponent::key);

        WHEN("targeting a unit or a tile")
        {
            std::shared_ptr<Unit> target = EntityTestUtils::makeUnit();
            world.addEntity(target, 2, 1);

            attackComponent->setTargetEntity(target);
            attackComponent->setTargetTile({ 3, 3 });
            attacker->update(TimeUtils::timeStepMs);

            THEN("no attack is initiated")
            {
                REQUIRE(attacker->getState() == UnitState::Idle);
            }
        }
    }
}

SCENARIO("AttackComponent does not interrupt movement", "[components][attack-component]")
{
    GIVEN("A unit with an AttackComponent that is currently moving")
    {
        World world(5, 5, false);

        std::shared_ptr<Unit> attacker = EntityTestUtils::makeUnit();
        world.addEntity(attacker, 1, 1);
        world.onAllEntitiesCreated();

        AttackComponent* attackComponent = attacker->requireComponent<AttackComponent>(AttackComponent::key);
        AttackDef attackDef;
        attackDef.range = 1;
        attackComponent->registerAttack(&attackDef);

        MovementComponent* movementComponent = attacker->requireComponent<MovementComponent>(MovementComponent::key);
        Pathfinding::Context context;
        movementComponent->moveTo({ 3, 3 }, context);

        attacker->update(TimeUtils::timeStepMs);

        WHEN("targeting a unit in range")
        {
            std::shared_ptr<Unit> target = EntityTestUtils::makeUnit();
            world.addEntity(target, 2, 1);

            attackComponent->setTargetEntity(target);

            THEN("an attack is NOT initiated")
            {
                REQUIRE(attacker->getState() == UnitState::Moving);
            }
        }
    }
}

}  // namespace TestAttackComponent
