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

    std::map<BuildingType, Spritesheet> Resources::initBuildingSpritesheets() {
        return std::map<BuildingType, Spritesheet>();
    }

    std::map<UnitType, Spritesheet> Resources::initUnitSpritesheets() {
        return std::map<UnitType, Spritesheet>();
    }

    std::map<int, Spritesheet> Resources::initTileSpritesheets() {
        return std::map<int, Spritesheet> {
            { 0, Spritesheet(Texture(0, 0, 0), 64, 64) }
        };
    }

    Spritesheet Resources::initMapBorderSpritesheet() {
        return Spritesheet(Texture(0, 0, 0), 64, 64);
    }

    const Spritesheet& Resources::getTileSpritesheet(bool wilderness) const {
        return tileSpritesheets.at(0);
    }

    const std::map<UnitType, Spritesheet>& Resources::getUnitSpritesheets() const {
        return unitSpritesheets;
    }

    const std::map<BuildingType, Spritesheet>& Resources::getBuildingSpritesheets() const {
        return buildingSpritesheets;
    }

    const Spritesheet& Resources::getMapBorderSpritesheet() const {
        return mapBorderSpritesheet;
    }

    const Texture& Resources::getPalette() const {
        return paletteTexture;
    }

}  // namespace Rival
