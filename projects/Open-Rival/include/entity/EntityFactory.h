#pragma once

#include <memory>
#include <random>

#include "application/Resources.h"
#include "audio/AudioSystem.h"
#include "entity/Entity.h"
#include "game/AttackDef.h"
#include "game/BuildingType.h"
#include "game/EffectDef.h"
#include "game/MapUtils.h"
#include "game/ProjectileDef.h"
#include "game/UnitType.h"
#include "scenario/ScenarioData.h"

namespace Rival {

class EntityFactory : public std::enable_shared_from_this<EntityFactory>
{
public:
    EntityFactory(const Resources& resources, AudioSystem& audioSystem, std::shared_ptr<std::mt19937> randomizer);

    /** Creates a Unit from raw data (e.g. read from a Scenario file). */
    std::shared_ptr<Entity> createUnit(const UnitPlacement& unitPlacement) const;

    /** Creates a Building from raw data (e.g. read from a Scenario file). */
    std::shared_ptr<Entity> createBuilding(const BuildingPlacement& buildingPlacement) const;

    /** Creates a Palisade from raw data (e.g. read from a Scenario file). */
    std::shared_ptr<Entity> createPalisade(
            const BuildingPlacement& buildingPlacement, bool wilderness, bool isGrate) const;

    /** Creates a Decoration from raw data (e.g. read from a Scenario file). */
    std::shared_ptr<Entity> createDecoration(const ObjectPlacement& objPlacement, bool wilderness) const;

    /** Creates a Projectile with the given properties. */
    std::shared_ptr<Entity> createProjectile(const AttackDef& attack,
            const ProjectileDef& projectileDef,
            const MapNode& pos,
            const MapNode& target) const;

    /** Creates an Effect at the given location. */
    std::shared_ptr<Entity> createEffect(
            const EffectDef& effectDef, Facing facing = Facing::North, float offsetY = 0.f) const;

private:
    UnitType getUnitType(std::uint8_t unitType) const;
    bool isNameEmpty(const std::string& name) const;
    Facing getFacing(std::uint8_t facing) const;
    BuildingType getBuildingType(std::uint8_t buildingType) const;

private:
    const Resources& resources;
    AudioSystem& audioSystem;
    std::shared_ptr<std::mt19937> randomizer;
};

}  // namespace Rival
