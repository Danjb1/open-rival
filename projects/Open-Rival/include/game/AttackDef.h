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
    std::string name;
    int damage = 0;
    int range = 0;
    int accuracy = 0;
    int projectileSpeed = 0;
    int manaCost = 0;
    int reloadTime = 0;
    std::string sound;
    bool splashDamage = false;
};

}  // namespace Rival
