#include "entity/EntityFactory.h"

#include <stdexcept>
#include <string>

#include "application/Resources.h"
#include "entity/Building.h"
#include "entity/Decoration.h"
#include "entity/Palisade.h"
#include "entity/Unit.h"
#include "entity/components/BuildingAnimationComponent.h"
#include "entity/components/FlyerComponent.h"
#include "entity/components/HealthComponent.h"
#include "entity/components/MouseHandlerComponent.h"
#include "entity/components/OwnerComponent.h"
#include "entity/components/PassabilityComponent.h"
#include "entity/components/PortraitComponent.h"
#include "entity/components/SeafarerComponent.h"
#include "entity/components/SpriteComponent.h"
#include "entity/components/UnitAnimationComponent.h"
#include "entity/components/VoiceComponent.h"
#include "entity/components/WalkerComponent.h"
#include "entity/components/WallComponent.h"
#include "game/Animations.h"
#include "game/InventoryComponent.h"
#include "game/PlayerState.h"
#include "game/Tile.h"
#include "game/UnitType.h"

namespace Rival {

EntityFactory::EntityFactory(const Resources& resources, AudioSystem& audioSystem)
    : resources(resources)
    , audioSystem(audioSystem)
{
}

std::shared_ptr<Entity> EntityFactory::createUnit(const UnitPlacement& unitPlacement) const
{
    // Find the UnitDef
    const UnitType unitType = getUnitType(unitPlacement.type);
    const UnitDef* unitDef = resources.getUnitDef(unitType);
    if (!unitDef)
    {
        throw std::runtime_error("No unit definition found for " + std::to_string(unitPlacement.type));
    }

    // Create Unit
    std::string name;
    bool isNameUnique;
    if (isNameEmpty(unitPlacement.name))
    {
        name = unitDef->name;
        isNameUnique = false;
    }
    else
    {
        name = unitPlacement.name;
        isNameUnique = true;
    }
    std::shared_ptr<Unit> unit = std::make_shared<Unit>(unitType, name, isNameUnique);

    // HealthComponent
    unit->attach(std::make_shared<HealthComponent>(unitPlacement.hitpoints));

    // OwnerComponent (note: monsters use player = 8)
    if (unitPlacement.player < PlayerStore::maxPlayers)
    {
        unit->attach(std::make_shared<OwnerComponent>(unitPlacement.player));
    }

    // FacingComponent
    const Facing facing = getFacing(unitPlacement.facing);
    unit->attach(std::make_shared<FacingComponent>(facing));

    // SpriteComponent
    const Spritesheet& spritesheet = resources.getUnitSpritesheet(unitType);
    unit->attach(std::make_shared<SpriteComponent>(spritesheet));

    // UnitAnimationComponent
    unit->attach(std::make_shared<UnitAnimationComponent>(*unitDef));

    // MovementComponent
    if (unitDef->movementMode == MovementMode::Flying)
    {
        unit->attach(std::make_shared<PassabilityComponent>(TilePassability::FlyingUnit));
        unit->attach(std::make_shared<FlyerComponent>());
    }
    else if (unitDef->movementMode == MovementMode::Seafaring)
    {
        unit->attach(std::make_shared<PassabilityComponent>(TilePassability::GroundUnit));
        unit->attach(std::make_shared<SeafarerComponent>());
    }
    else
    {
        unit->attach(std::make_shared<PassabilityComponent>(TilePassability::GroundUnit));
        unit->attach(std::make_shared<WalkerComponent>());
    }

    // VoiceComponent
    unit->attach(std::make_shared<VoiceComponent>(resources, audioSystem, *unitDef));

    // MouseHandlerComponent
    unit->attach(std::make_shared<MouseHandlerComponent>());

    // InventoryComponent
    unit->attach(std::make_shared<InventoryComponent>());

    // PortraitComponent
    unit->attach(std::make_shared<PortraitComponent>(unitDef->portraitId));

    return unit;
}

std::shared_ptr<Entity> EntityFactory::createBuilding(const BuildingPlacement& buildingPlacement) const
{
    // Create Building
    const BuildingType buildingType = getBuildingType(buildingPlacement.type);
    std::shared_ptr<Building> building = std::make_shared<Building>(buildingType);

    // Find the BuildingDef
    const BuildingDef* buildingDef = resources.getBuildingDef(buildingType);
    if (!buildingDef)
    {
        throw std::runtime_error("No building definition found for " + std::to_string(buildingPlacement.type));
    }

    // HealthComponent
    building->attach(std::make_shared<HealthComponent>(buildingPlacement.hitpoints));

    // OwnerComponent
    building->attach(std::make_shared<OwnerComponent>(buildingPlacement.player));

    // SpriteComponent
    const Spritesheet& spritesheet = resources.getBuildingSpritesheet(buildingType);
    building->attach(std::make_shared<SpriteComponent>(spritesheet));

    if (building->isWall())
    {
        // WallComponent
        WallVariant wallVariant = static_cast<WallVariant>(buildingPlacement.wallVariant);
        building->attach(std::make_shared<WallComponent>(wallVariant));
    }
    else
    {
        // BuildingAnimationComponent
        building->attach(std::make_shared<BuildingAnimationComponent>(*buildingDef));
    }

    // PassabilityComponent
    building->attach(std::make_shared<PassabilityComponent>(TilePassability::Building));

    return building;
}

std::shared_ptr<Entity> EntityFactory::createPalisade(
        const BuildingPlacement& buildingPlacement, bool wilderness, bool isGrate) const
{
    // Create Entity
    std::shared_ptr<Palisade> building = std::make_shared<Palisade>(isGrate);

    // SpriteComponent
    const Spritesheet& spritesheet = resources.getObjectSpritesheet(wilderness);
    building->attach(std::make_shared<SpriteComponent>(spritesheet));

    // WallComponent
    WallVariant wallVariant = static_cast<WallVariant>(buildingPlacement.wallVariant);
    building->attach(std::make_shared<WallComponent>(wallVariant));

    // PassabilityComponent
    building->attach(std::make_shared<PassabilityComponent>(TilePassability::Building));

    return building;
}

std::shared_ptr<Entity> EntityFactory::createObject(const ObjectPlacement& objPlacement, bool wilderness) const
{
    // Create Entity
    std::shared_ptr<Decoration> obj = std::make_shared<Decoration>();

    // SpriteComponent
    if (objPlacement.type == 0xAF)
    {
        const Spritesheet& spritesheet = resources.getCommonObjectSpritesheet();
        obj->attach(std::make_shared<SpriteComponent>(spritesheet));
    }
    else
    {
        const Spritesheet& spritesheet = resources.getObjectSpritesheet(wilderness);
        obj->attach(std::make_shared<SpriteComponent>(spritesheet));
    }

    // TODO: AnimationComponent
    // const Animation anim = getObjectAnimation(objPlacement.type, objPlacement.variant);
    // obj->attach(std::make_shared<AnimationComponent>(anim));

    return obj;
}

UnitType EntityFactory::getUnitType(std::uint8_t unitType) const
{
    switch (unitType)
    {
    case 0x33:
        return UnitType::Peasant;
    case 0x34:
        return UnitType::Bowman;
    case 0x35:
        return UnitType::LightCavalry;
    case 0x36:
        return UnitType::Knight;
    case 0x37:
        return UnitType::FireMaster;
    case 0x38:
        return UnitType::Thief;
    case 0x39:
        return UnitType::Ballista;
    case 0x3A:
        return UnitType::ChariotOfWar;
    case 0x3B:
        return UnitType::Wizard;
    case 0x3C:
        return UnitType::Priest;
    case 0x3D:
        return UnitType::SeaBarge;
    case 0x3E:
        return UnitType::Battleship;
    case 0x3F:
        return UnitType::PegasRider;
    case 0x40:
        return UnitType::Zeppelin;
    case 0x41:
        return UnitType::Serf;
    case 0x42:
        return UnitType::RockThrower;
    case 0x43:
        return UnitType::HordeRider;
    case 0x44:
        return UnitType::Warlord;
    case 0x45:
        return UnitType::GnomeBoomer;
    case 0x46:
        return UnitType::Rogue;
    case 0x47:
        return UnitType::Catapult;
    case 0x48:
        return UnitType::StormTrooper;
    case 0x49:
        return UnitType::PriestOfDoom;
    case 0x4A:
        return UnitType::Necromancer;
    case 0x4B:
        return UnitType::LandingCraft;
    case 0x4C:
        return UnitType::TrollGalley;
    case 0x4D:
        return UnitType::Warbat;
    case 0x4E:
        return UnitType::Balloon;
    case 0x4F:
        return UnitType::Yeoman;
    case 0x50:
        return UnitType::Archer;
    case 0x51:
        return UnitType::Druid;
    case 0x52:
        return UnitType::Centaur;
    case 0x53:
        return UnitType::DwarfMiner;
    case 0x54:
        return UnitType::Scout;
    case 0x55:
        return UnitType::Bombard;
    case 0x56:
        return UnitType::Arquebusier;
    case 0x57:
        return UnitType::Mage;
    case 0x58:
        return UnitType::Enchanter;
    case 0x59:
        return UnitType::Bark;
    case 0x5A:
        return UnitType::Warship;
    case 0x5B:
        return UnitType::SkyRider;
    case 0x5C:
        return UnitType::MagicChopper;
    case 0x5D:
        return UnitType::SeaMonster;  // Black
    case 0x5E:
        return UnitType::SeaMonster;  // Blue
    case 0x5F:
        return UnitType::SeaMonster;  // Green
    case 0x60:
        return UnitType::Snake;  // Black
    case 0x61:
        return UnitType::Snake;  // Red
    case 0x62:
        return UnitType::Snake;  // Green
    case 0x63:
        return UnitType::Snake;  // Yellow
    case 0x64:
        return UnitType::Gryphon;  // Black
    case 0x65:
        return UnitType::Gryphon;  // Yellow
    case 0x66:
        return UnitType::Gryphon;  // Cyan
    case 0x67:
        return UnitType::Hydra;  // Black
    case 0x68:
        return UnitType::Hydra;  // Blue
    case 0x69:
        return UnitType::Hydra;  // Green
    case 0x6A:
        return UnitType::Golem;  // Black
    case 0x6B:
        return UnitType::Golem;  // Blue
    case 0x6C:
        return UnitType::Golem;  // Cyan
    case 0x6D:
        return UnitType::Devil;  // Black
    case 0x6E:
        return UnitType::Devil;  // Red
    case 0x6F:
        return UnitType::Devil;  // Orange
    case 0x70:
        return UnitType::Devil;  // Yellow
    case 0x71:
        return UnitType::Skeleton;  // Black
    case 0x72:
        return UnitType::Skeleton;  // Red
    case 0x73:
        return UnitType::Skeleton;  // Cyan
    case 0x74:
        return UnitType::Skeleton;  // Yellow
    case 0x75:
        return UnitType::Dragon;  // Black
    case 0x76:
        return UnitType::Dragon;  // Red
    case 0x77:
        return UnitType::Dragon;  // Violet
    case 0x78:
        return UnitType::Dragon;  // Blue
    case 0x79:
        return UnitType::Dragon;  // Orange
    case 0x7A:
        return UnitType::Dragon;  // Green
    default:
        throw std::runtime_error("Unknown unit type: " + std::to_string(unitType));
    }
}

bool EntityFactory::isNameEmpty(const std::string& name) const
{
    return name[0] == '\0';
}

Facing EntityFactory::getFacing(std::uint8_t facing) const
{
    switch (facing)
    {
    case 0:
        return Facing::South;
    case 1:
        return Facing::SouthWest;
    case 2:
        return Facing::West;
    case 3:
        return Facing::NorthWest;
    case 4:
        return Facing::North;
    case 5:
        return Facing::NorthEast;
    case 6:
        return Facing::East;
    case 7:
        return Facing::SouthEast;
    default:
        throw std::runtime_error("Unknown facing: " + std::to_string(facing));
    }
}

BuildingType EntityFactory::getBuildingType(std::uint8_t buildingType) const
{
    switch (buildingType)
    {
    case 0x27:
        return BuildingType::ElvenKeep;
    case 0x28:
        return BuildingType::Treasury;
    case 0x29:
        return BuildingType::CombatCamp;
    case 0x2a:
        return BuildingType::Arsenal;
    case 0x2b:
        return BuildingType::DuelRange;
    case 0x2c:
        return BuildingType::HolyNest;
    case 0x2d:
        return BuildingType::MinerGuildhall;
    case 0x2e:
        return BuildingType::AbbeyTower;
    case 0x2f:
        return BuildingType::CouncilOfRunes;
    case 0x30:
        return BuildingType::Harbour;
    case 0x31:
        return BuildingType::WarningTower;
    case 0x32:
        return BuildingType::TreeWall;
    case 0x1b:
        return BuildingType::Fortress;
    case 0x1c:
        return BuildingType::HoardKeep;
    case 0x1d:
        return BuildingType::Fort;
    case 0x1e:
        return BuildingType::Blacksmith;
    case 0x1f:
        return BuildingType::BattleQuarters;
    case 0x20:
        return BuildingType::BlackNest;
    case 0x21:
        return BuildingType::WeirdWorkshop;
    case 0x22:
        return BuildingType::UnholyChapel;
    case 0x23:
        return BuildingType::AltarOfDoom;
    case 0x24:
        return BuildingType::Docks;
    case 0x25:
        return BuildingType::GuardTower;
    case 0x26:
        return BuildingType::GreenskinWall;
    case 0x0f:
        return BuildingType::Castle;
    case 0x10:
        return BuildingType::GoldMill;
    case 0x11:
        return BuildingType::ArcheryRange;
    case 0x12:
        return BuildingType::Armoury;
    case 0x13:
        return BuildingType::Barracks;
    case 0x14:
        return BuildingType::HolyStables;
    case 0x15:
        return BuildingType::FireGuild;
    case 0x16:
        return BuildingType::Temple;
    case 0x17:
        return BuildingType::MageTower;
    case 0x18:
        return BuildingType::Shipyard;
    case 0x19:
        return BuildingType::WatchTower;
    case 0x1a:
        return BuildingType::Wall;
    default:
        throw std::runtime_error("Unknown building type: " + std::to_string(buildingType));
    }
}

}  // namespace Rival
