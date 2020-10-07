#include "pch.h"
#include "EntityFactory.h"

#include "Animations.h"
#include "AnimationComponent.h"
#include "BuildingPropsComponent.h"
#include "OwnerComponent.h"
#include "PassabilityComponent.h"
#include "SpriteComponent.h"
#include "Tile.h"
#include "UnitPropsComponent.h"
#include "WallComponent.h"

namespace Rival {

    EntityFactory::EntityFactory(const Resources& res)
        : res(res) {}

    std::unique_ptr<Entity> EntityFactory::createUnit(
            const UnitPlacement& unitPlacement) const {

        // Create Unit
        std::unique_ptr<Entity> unit = std::make_unique<Entity>(
                Unit::width, Unit::height);

        // Add UnitPropsComponent
        const Unit::Type unitType = getUnitType(unitPlacement.type);
        unit->attach(std::make_unique<UnitPropsComponent>(unitType));

        // Add OwnerComponent
        unit->attach(std::make_unique<OwnerComponent>(unitPlacement.player));

        // Add FacingComponent
        const Facing facing = getFacing(unitPlacement.facing);
        unit->attach(std::make_unique<FacingComponent>(facing));

        // Add SpriteComponent
        const Spritesheet& spritesheet = res.getUnitSpritesheet(unitType);
        unit->attach(std::make_unique<SpriteComponent>(spritesheet));

        // Add AnimationComponent
        const Animations::Animation anim = Animations::getUnitAnimation(
                unitType, Animations::UnitAnimationType::Standing);
        unit->attach(std::make_unique<AnimationComponent>(anim));

        // Add Passability
        // TODO: consider flying units separately
        unit->attach(std::make_unique<PassabilityComponent>(
                TilePassability::Unit));

        return unit;
    }

    std::unique_ptr<Entity> EntityFactory::createBuilding(
            const BuildingPlacement& buildingPlacement) const {

        // Create Building
        Building::Type buildingType =
                getBuildingType(buildingPlacement.type);
        int width = Building::getWidth(buildingType);
        int height = Building::getHeight(buildingType);
        std::unique_ptr<Entity> building = std::make_unique<Entity>(
                width, height);

        // Add BuildingPropsComponent
        building->attach(
                std::make_unique<BuildingPropsComponent>(buildingType));

        // Add OwnerComponent
        building->attach(std::make_unique<OwnerComponent>(
                buildingPlacement.player));

        // Add SpriteComponent
        const Spritesheet& spritesheet =
                res.getBuildingSpritesheet(buildingType);
        building->attach(std::make_unique<SpriteComponent>(spritesheet));

        if (Building::isWall(buildingType)) {
            // Add WallComponent
            WallVariant wallVariant = static_cast<WallVariant>(
                    buildingPlacement.wallVariant);
            building->attach(std::make_unique<WallComponent>(wallVariant));

        } else {
            // Add AnimationComponent
            const Animations::Animation anim = Animations::getBuildingAnimation(
                    buildingType, Animations::BuildingAnimationType::Built);
            building->attach(std::make_unique<AnimationComponent>(anim));
        }

        // Add Passability
        building->attach(std::make_unique<PassabilityComponent>(
                TilePassability::Building));

        return building;
    }

    Unit::Type EntityFactory::getUnitType(std::uint8_t unitType) const {
        switch (unitType) {
        case 0x33:
            return Unit::Type::Peasant;
        case 0x34:
            return Unit::Type::Bowman;
        case 0x35:
            return Unit::Type::LightCavalry;
        case 0x36:
            return Unit::Type::Knight;
        case 0x37:
            return Unit::Type::FireMaster;
        case 0x38:
            return Unit::Type::Thief;
        case 0x39:
            return Unit::Type::Ballista;
        case 0x3A:
            return Unit::Type::ChariotOfWar;
        case 0x3B:
            return Unit::Type::Wizard;
        case 0x3C:
            return Unit::Type::Priest;
        case 0x3D:
            return Unit::Type::SeaBarge;
        case 0x3E:
            return Unit::Type::Battleship;
        case 0x3F:
            return Unit::Type::PegasRider;
        case 0x40:
            return Unit::Type::Zeppelin;
        case 0x41:
            return Unit::Type::Serf;
        case 0x42:
            return Unit::Type::RockThrower;
        case 0x43:
            return Unit::Type::HordeRider;
        case 0x44:
            return Unit::Type::Warlord;
        case 0x45:
            return Unit::Type::GnomeBoomer;
        case 0x46:
            return Unit::Type::Rogue;
        case 0x47:
            return Unit::Type::Catapult;
        case 0x48:
            return Unit::Type::StormTrooper;
        case 0x49:
            return Unit::Type::PriestOfDoom;
        case 0x4A:
            return Unit::Type::Necromancer;
        case 0x4B:
            return Unit::Type::LandingCraft;
        case 0x4C:
            return Unit::Type::TrollGalley;
        case 0x4D:
            return Unit::Type::Warbat;
        case 0x4E:
            return Unit::Type::Balloon;
        case 0x4F:
            return Unit::Type::Yeoman;
        case 0x50:
            return Unit::Type::Archer;
        case 0x51:
            return Unit::Type::Druid;
        case 0x52:
            return Unit::Type::Centaur;
        case 0x53:
            return Unit::Type::DwarfMiner;
        case 0x54:
            return Unit::Type::Scout;
        case 0x55:
            return Unit::Type::Bombard;
        case 0x56:
            return Unit::Type::Arquebusier;
        case 0x57:
            return Unit::Type::Mage;
        case 0x58:
            return Unit::Type::Enchanter;
        case 0x59:
            return Unit::Type::Bark;
        case 0x60:
            return Unit::Type::Warship;
        case 0x5B:
            return Unit::Type::SkyRider;
        case 0x5C:
            return Unit::Type::MagicChopper;
        default:
            throw std::runtime_error("Unknown unit type: " + unitType);
        }
    }

    Facing EntityFactory::getFacing(std::uint8_t facing) const {
        switch (facing) {
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
            throw std::runtime_error("Unknown facing: " + facing);
        }
    }

    Building::Type EntityFactory::getBuildingType(
            std::uint8_t buildingType) const {
        switch (buildingType) {
        case 0x27:
            return Building::Type::ElvenKeep;
        case 0x28:
            return Building::Type::Treasury;
        case 0x29:
            return Building::Type::CombatCamp;
        case 0x2a:
            return Building::Type::Arsenal;
        case 0x2b:
            return Building::Type::DuelRange;
        case 0x2c:
            return Building::Type::HolyNest;
        case 0x2d:
            return Building::Type::MinerGuildhall;
        case 0x2e:
            return Building::Type::AbbeyTower;
        case 0x2f:
            return Building::Type::CouncilOfRunes;
        case 0x30:
            return Building::Type::Harbour;
        case 0x31:
            return Building::Type::WarningTower;
        case 0x32:
            return Building::Type::TreeWall;
        case 0x1b:
            return Building::Type::Fortress;
        case 0x1c:
            return Building::Type::HoardKeep;
        case 0x1d:
            return Building::Type::Fort;
        case 0x1e:
            return Building::Type::Blacksmith;
        case 0x1f:
            return Building::Type::BattleQuarters;
        case 0x20:
            return Building::Type::BlackNest;
        case 0x21:
            return Building::Type::WeirdWorkshop;
        case 0x22:
            return Building::Type::UnholyChapel;
        case 0x23:
            return Building::Type::AltarOfDoom;
        case 0x24:
            return Building::Type::Docks;
        case 0x25:
            return Building::Type::GuardTower;
        case 0x26:
            return Building::Type::GreenskinWall;
        case 0x0f:
            return Building::Type::Castle;
        case 0x10:
            return Building::Type::GoldMill;
        case 0x11:
            return Building::Type::ArcheryRange;
        case 0x12:
            return Building::Type::Armoury;
        case 0x13:
            return Building::Type::Barracks;
        case 0x14:
            return Building::Type::HolyStables;
        case 0x15:
            return Building::Type::FireGuild;
        case 0x16:
            return Building::Type::Temple;
        case 0x17:
            return Building::Type::MageTower;
        case 0x18:
            return Building::Type::Shipyard;
        case 0x19:
            return Building::Type::WatchTower;
        case 0x1a:
            return Building::Type::Wall;
        default:
            throw std::runtime_error("Unknown building type: " + unsigned(buildingType));
        }
    }

}  // namespace Rival
