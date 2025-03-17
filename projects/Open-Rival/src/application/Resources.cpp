#include "application/Resources.h"

#include <stdexcept>
#include <string>

#include "audio/MidiFile.h"
#include "audio/Sounds.h"
#include "audio/WaveFile.h"
#include "game/UnitType.h"
#include "gfx/Font.h"
#include "gfx/Spritesheet.h"
#include "gfx/Texture.h"
#include "gfx/TextureAtlas.h"
#include "utils/JsonUtils.h"

namespace Rival {

std::shared_ptr<const Font> Resources::getFontSmall() const
{
    return fontSmall;
}

std::shared_ptr<const Font> Resources::getFontRegular() const
{
    return fontRegular;
}

std::shared_ptr<const Spritesheet> Resources::getTileSpritesheet(bool wilderness) const
{
    return wilderness ? tileSpritesheets.at(1) : tileSpritesheets.at(0);
}

std::shared_ptr<const Spritesheet> Resources::getUnitSpritesheet(UnitType unitType) const
{
    return unitSpritesheets.at(unitType);
}

std::shared_ptr<const Spritesheet> Resources::getProjectileSpritesheet(const std::string& name) const
{
    return projectileSpritesheets.at(name);
}

std::shared_ptr<const Spritesheet> Resources::getEffectSpritesheet(const std::string& name) const
{
    return effectSpritesheets.at(name);
}

std::shared_ptr<const Spritesheet> Resources::getBuildingSpritesheet(BuildingType buildingType) const
{
    return buildingSpritesheets.at(buildingType);
}

std::shared_ptr<const Spritesheet> Resources::getCommonObjectSpritesheet() const
{
    return objectSpritesheets.at(0);
}

std::shared_ptr<const Spritesheet> Resources::getObjectSpritesheet(bool wilderness) const
{
    return wilderness ? objectSpritesheets.at(2) : objectSpritesheets.at(1);
}

std::shared_ptr<const Spritesheet> Resources::getCursorSpritesheet() const
{
    return cursorSpritesheet;
}

std::shared_ptr<const Spritesheet> Resources::getMapBorderSpritesheet() const
{
    return mapBorderSpritesheet;
}

std::shared_ptr<const Spritesheet> Resources::getPortraitSpritesheet() const
{
    return portraitSpritesheet;
}

std::shared_ptr<const Spritesheet> Resources::getHitboxSpritesheet() const
{
    return hitboxSpritesheet;
}

std::shared_ptr<const Texture> Resources::getPalette() const
{
    return paletteTexture;
}

std::shared_ptr<const TextureAtlas> Resources::getUiTextureAtlas() const
{
    return textureAtlases.at(0);
}

std::shared_ptr<const TextureAtlas> Resources::getOverlayTextureAtlas() const
{
    return textureAtlases.at(1);
}

std::shared_ptr<const Texture> Resources::getMenuBackgroundTexture() const
{
    return textures.at(ResourceLoader::txIndexMenuBackground);
}

std::shared_ptr<const Texture> Resources::getTexture(int id) const
{
    return textures.at(id);
}

std::shared_ptr<const WaveFile> Resources::getSound(int id) const
{
    return sounds.at(id);
}

std::vector<std::shared_ptr<const WaveFile>> Resources::getAllSounds() const
{
    return sounds;
}

std::shared_ptr<const MidiFile> Resources::getMidi(int id) const
{
    return midis.at(id);
}

const SoundBank* Resources::getSoundBank(const std::string& soundId) const
{
    auto iter = soundBanks.find(soundId);
    return iter == soundBanks.cend() ? nullptr : &iter->second;
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

const AttackDef* Resources::getAttackDef(int attackId) const
{
    auto iter = attackDefs.find(attackId);
    return iter == attackDefs.cend() ? nullptr : &iter->second;
}

const ProjectileDef* Resources::getProjectileDef(const std::string& projectileName) const
{
    auto iter = projectileDefs.find(projectileName);
    return iter == projectileDefs.cend() ? nullptr : &iter->second;
}

const EffectDef* Resources::getEffectDef(const std::string& effectName) const
{
    auto iter = effectDefs.find(effectName);
    return iter == effectDefs.cend() ? nullptr : &iter->second;
}

}  // namespace Rival
