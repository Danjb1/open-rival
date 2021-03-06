#include "pch.h"
#include "Resources.h"

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

namespace Rival {

    const std::string Resources::mapsDir = "res\\maps\\";
    const std::string Resources::txDir = "res\\textures\\";

    Resources::Resources()
        : textures(loadTextures()),
          paletteTexture(initPaletteTexture()),
          unitSpritesheets(initUnitSpritesheets()),
          buildingSpritesheets(initBuildingSpritesheets()),
          tileSpritesheets(initTileSpritesheets()),
          mapBorderSpritesheet(initMapBorderSpritesheet()) {}

    Resources::~Resources() {}

    std::vector<Texture> Resources::loadTextures() {
        return std::vector<Texture>();
    }

    Texture Resources::initPaletteTexture() {
        return Texture(0, 256, 1);
    }

    std::map<Unit::Type, Spritesheet> Resources::initUnitSpritesheets() {
        return std::map<Unit::Type, Spritesheet> {
            { Unit::Type::Knight, Spritesheet(Texture(0, 0, 0), 64, 64) }
        };
    }

    std::map<Building::Type, Spritesheet> Resources::initBuildingSpritesheets() {
        return std::map<Building::Type, Spritesheet> {
            { Building::Type::Barracks, Spritesheet(Texture(0, 0, 0), 64, 64) }
        };
    }

    std::vector<Spritesheet> Resources::initTileSpritesheets() {
        return std::vector<Spritesheet> {
            { Spritesheet(Texture(0, 0, 0), 64, 64) }
        };
    }

    Spritesheet Resources::initMapBorderSpritesheet() {
        return Spritesheet(Texture(0, 0, 0), 64, 64);
    }

    const Spritesheet& Resources::getTileSpritesheet(bool wilderness) const {
        return tileSpritesheets.at(0);
    }

    const Spritesheet& Resources::getUnitSpritesheet(
            Unit::Type unitType) const {
        return unitSpritesheets.at(Unit::Type::Knight);
    }

    const Spritesheet& Resources::getBuildingSpritesheet(
            Building::Type buildingType) const {
        return buildingSpritesheets.at(Building::Type::Barracks);
    }

    const Spritesheet& Resources::getMapBorderSpritesheet() const {
        return mapBorderSpritesheet;
    }

    const Texture& Resources::getPalette() const {
        return paletteTexture;
    }

}  // namespace Rival
