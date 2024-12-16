#pragma once

#include <string>

#include "utils/JsonUtils.h"

namespace Rival {

class AttackDef final
{
public:
    static AttackDef fromJson(const json& j);

    AttackDef(std::string name)
        : name(name)
    {
    }

public:
    /** Name of the attack (not shown in-game). */
    std::string name;

    /** Maximum damage dealt by the attack (depending on accuracy).
     * For ranged attacks, this is only dealt if/when the projectile hits. */
    int damage = 0;

    /** Range of the attack.
     * For melee attacks, this will be 1. */
    int range = 0;

    /** Accuracy of the attack (0-100).
     * This is used to determine how much of the damage is dealt.
     * - At 100 Accuracy, 100% of damage is dealt, always.
     * - At 50 Accuracy, 50-100% of damage is dealt.
     */
    int accuracy = 0;

    /** Speed of the projectile spawned by this attack (only relevant for ranged attacks).
     * This is based on the "Move Space" and "Move Time" properties taken from the scenario editor. */
    int projectileSpeed = 0;

    /** Mana cost of the attack. */
    int manaCost = 0;

    /** Reload time between attacks, in ms.
     * Even though the Scenario Editor gives these values in milliseconds, there seems to be some discrepancy compared
     * to the measured values in-game. For example, the Wizard's Flash Light has a Reload Time of 760ms, but in practice
     * it only fires every ~1200ms. */
    int reloadTime = 0;

    /** Name of the projectile to spawn when the attack is initiated (for projectile attacks). */
    std::string projectile;

    /** Name of the sound to play when the attack is initiated. */
    std::string sound;

    /** Name of the sound to play when the attack hits (only relevant for projectiles). */
    std::string impactSound;

    /** Whether this attack's projectile should damage to the surrounding tiles (only relevant for projectiles). */
    bool hasSplashDamage = false;
};

}  // namespace Rival
