#include "catch2/catch.h"

#include <array>

#include "entity/Unit.h"
#include "entity/components/AttackComponent.h"
#include "entity/components/MovementComponent.h"
#include "game/Pathfinding.h"
#include "game/World.h"
#include "utils/EntityTestUtils.h"
#include "utils/LogUtils.h"
#include "utils/TimeUtils.h"

using namespace Rival;

// Adapted from Main.cpp
static void initLogging()
{
    static bool isLogInitialized = false;

    if (isLogInitialized)
    {
        return;
    }

    LogUtils::initLogging("info", true, false, "Open-Rival");
    LogUtils::makeLogCategory("attack", "info");
    LogUtils::makeLogCategory("movement", "info");
    LogUtils::makeLogCategory("pathfinding", "info");

    isLogInitialized = true;
}

namespace TestAttackComponent {

SCENARIO("AttackComponent can initiate an attack", "[components][attack-component]")
{
    GIVEN("A unit with an AttackComponent")
    {
        initLogging();
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

SCENARIO("AttackComponent does not interrupt movement", "[components][attack-component]")
{
    GIVEN("A unit with an AttackComponent that is currently moving")
    {
        initLogging();
        World world(5, 5, false);

        std::shared_ptr<Unit> attacker = EntityTestUtils::makeUnit();
        world.addEntity(attacker, 1, 1);

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
