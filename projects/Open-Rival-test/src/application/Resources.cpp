#include "application/Resources.h"

#include "application/ApplicationContext.h"

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

namespace Rival {

const std::string Resources::mapsDir = "res\\maps\\";
const std::string Resources::txDir = "res\\textures\\";

Resources::Resources(ApplicationContext& context)
    : context(context)
    , fontSmall(initFontSmall())
    , fontRegular(initFontRegular())
    , textures(loadTextures())
    , textureAtlases(loadTextureAtlases())
    , paletteTexture(initPaletteTexture())
    , unitSpritesheets(initUnitSpritesheets())
    , buildingSpritesheets(initBuildingSpritesheets())
    , tileSpritesheets(initTileSpritesheets())
    , objectSpritesheets(initObjectSpritesheets())
    , cursorSpritesheet(initCursorSpritesheet())
    , mapBorderSpritesheet(initMapBorderSpritesheet())
    , portraitSpritesheet(initPortraitSpritesheet())
    , hitboxSpritesheet(initHitboxSpritesheet())
    , sounds(initSounds())
    , midis(initMidis())
    , unitDefs(initUnitDefs())
    , buildingDefs(initBuildingDefs())

{
}

Font Resources::initFontSmall()
{
    FT_Library ft;
    return Font::loadFont(ft, {}, "", 0);
}

Font Resources::initFontRegular()
{
    FT_Library ft;
    return Font::loadFont(ft, {}, "", 0);
}

std::vector<std::shared_ptr<const Texture>> loadTextures()
{
    return {};
}

std::vector<std::shared_ptr<const TextureAtlas>> loadTextureAtlases()
{
    return {};
}

std::shared_ptr<const Texture> Resources::initPaletteTexture()
{
    return std::make_shared<const Texture>(0, 256, 1);
}

std::unordered_map<Building::Type, Spritesheet> Resources::initBuildingSpritesheets()
{
    return {};
}

std::unordered_map<Unit::Type, Spritesheet> Resources::initUnitSpritesheets()
{
    return {};
}

std::vector<Spritesheet> Resources::initTileSpritesheets()
{
    return {};
}

std::vector<Spritesheet> Resources::initObjectSpritesheets()
{
    return {};
}

Spritesheet Resources::initCursorSpritesheet()
{
    std::shared_ptr<Texture> tex = std::make_shared<Texture>(0, 0, 0);
    return Spritesheet(tex, 64, 64, 0);
}

Spritesheet Resources::initMapBorderSpritesheet()
{
    std::shared_ptr<Texture> tex = std::make_shared<Texture>(0, 0, 0);
    return Spritesheet(tex, 64, 64, 0);
}

Spritesheet Resources::initPortraitSpritesheet()
{
    std::shared_ptr<Texture> tex = std::make_shared<Texture>(0, 0, 0);
    return Spritesheet(tex, 64, 64, 0);
}

Spritesheet Resources::initHitboxSpritesheet()
{
    std::shared_ptr<Texture> tex = std::make_shared<Texture>(0, 0, 0);
    return Spritesheet(tex, 64, 64, 0);
}

std::vector<WaveFile> Resources::initSounds()
{
    return {};
}

std::vector<MidiFile> Resources::initMidis()
{
    return {};
}

std::unordered_map<Unit::Type, UnitDef> Resources::initUnitDefs() const
{
    return {};
}

std::unordered_map<Building::Type, BuildingDef> Resources::initBuildingDefs() const
{
    return {};
}

const Spritesheet& Resources::getTileSpritesheet(bool /*wilderness*/) const
{
    return tileSpritesheets.at(0);
}

const Spritesheet& Resources::getUnitSpritesheet(Unit::Type /*unitType*/) const
{
    return unitSpritesheets.at(Unit::Type::Knight);
}

const Spritesheet& Resources::getBuildingSpritesheet(Building::Type /*buildingType*/) const
{
    return buildingSpritesheets.at(Building::Type::Barracks);
}

const Spritesheet& Resources::getMapBorderSpritesheet() const
{
    return mapBorderSpritesheet;
}

std::shared_ptr<const Texture> Resources::getPalette() const
{
    return paletteTexture;
}

}  // namespace Rival
