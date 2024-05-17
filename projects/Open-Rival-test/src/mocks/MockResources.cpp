/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !!! Stub implementation for unit testing !!!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */

#include "application/ApplicationContext.h"
#include "application/Resources.h"

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

std::vector<std::shared_ptr<const Texture>> Resources::loadTextures()
{
    return {};
}

std::vector<std::shared_ptr<const TextureAtlas>> Resources::loadTextureAtlases()
{
    return {};
}

std::shared_ptr<const Texture> Resources::initPaletteTexture()
{
    return std::make_shared<const Texture>(0, 256, 1);
}

std::unordered_map<BuildingType, Spritesheet> Resources::initBuildingSpritesheets()
{
    return {};
}

std::unordered_map<UnitType, Spritesheet> Resources::initUnitSpritesheets()
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

std::unordered_map<UnitType, UnitDef> Resources::initUnitDefs() const
{
    return {};
}

std::unordered_map<BuildingType, BuildingDef> Resources::initBuildingDefs() const
{
    return {};
}

std::shared_ptr<const Texture> Resources::getPalette() const
{
    return paletteTexture;
}

const Spritesheet& Resources::getTileSpritesheet(bool /*wilderness*/) const
{
    return tileSpritesheets.at(0);
}

const Spritesheet& Resources::getUnitSpritesheet(UnitType /*unitType*/) const
{
    return unitSpritesheets.at(UnitType::Knight);
}

const Spritesheet& Resources::getBuildingSpritesheet(BuildingType /*buildingType*/) const
{
    return buildingSpritesheets.at(BuildingType::Barracks);
}

const Spritesheet& Resources::getCommonObjectSpritesheet() const
{
    return objectSpritesheets.at(0);
}

const Spritesheet& Resources::getObjectSpritesheet(bool /*wilderness*/) const
{
    return objectSpritesheets.at(0);
}

const Spritesheet& Resources::getCursorSpritesheet() const
{
    return cursorSpritesheet;
}

const Spritesheet& Resources::getMapBorderSpritesheet() const
{
    return mapBorderSpritesheet;
}

const Spritesheet& Resources::getPortraitSpritesheet() const
{
    return portraitSpritesheet;
}

const Spritesheet& Resources::getHitboxSpritesheet() const
{
    return hitboxSpritesheet;
}

std::shared_ptr<const TextureAtlas> Resources::getUiTextureAtlas() const
{
    return {};
}

std::shared_ptr<const Texture> Resources::getMenuBackgroundTexture() const
{
    return {};
}

const Font& Resources::getFontSmall() const
{
    return fontSmall;
}

const Font& Resources::getFontRegular() const
{
    return fontRegular;
}

const WaveFile& Resources::getSound(int id) const
{
    return sounds.at(id);
}

const MidiFile& Resources::getMidi(int id) const
{
    return midis.at(id);
}

const UnitDef* Resources::getUnitDef(UnitType unitType) const
{
    auto iter = unitDefs.find(unitType);
    return iter == unitDefs.cend() ? nullptr : &iter->second;
}

const BuildingDef* Resources::getBuildingDef(BuildingType buildingType) const
{
    auto iter = buildingDefs.find(buildingType);
    return iter == buildingDefs.cend() ? nullptr : &iter->second;
}

}  // namespace Rival
