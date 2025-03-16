#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "application/ResourceLoader.h"
#include "game/AttackDef.h"
#include "game/BuildingDef.h"
#include "game/BuildingType.h"
#include "game/EffectDef.h"
#include "game/ProjectileDef.h"
#include "game/UnitDef.h"
#include "game/UnitType.h"

namespace Rival {

class Font;
class MidiFile;
class SoundBank;
class Spritesheet;
class Texture;
class TextureAtlas;
class WaveFile;

/**
 * Interface providing access to fonts.
 */
class FontStore
{
public:
    virtual std::shared_ptr<const Font> getFontSmall() const = 0;
    virtual std::shared_ptr<const Font> getFontRegular() const = 0;
};

/**
 * Interface providing access to textures.
 */
class TextureStore
{
public:
    virtual std::shared_ptr<const Texture> getPalette() const = 0;
    virtual std::shared_ptr<const Spritesheet> getTileSpritesheet(bool wilderness) const = 0;
    virtual std::shared_ptr<const Spritesheet> getUnitSpritesheet(UnitType unitType) const = 0;
    virtual std::shared_ptr<const Spritesheet> getProjectileSpritesheet(const std::string& name) const = 0;
    virtual std::shared_ptr<const Spritesheet> getBuildingSpritesheet(BuildingType buildingType) const = 0;
    virtual std::shared_ptr<const Spritesheet> getCommonObjectSpritesheet() const = 0;
    virtual std::shared_ptr<const Spritesheet> getObjectSpritesheet(bool wilderness) const = 0;
    virtual std::shared_ptr<const Spritesheet> getCursorSpritesheet() const = 0;
    virtual std::shared_ptr<const Spritesheet> getMapBorderSpritesheet() const = 0;
    virtual std::shared_ptr<const Spritesheet> getPortraitSpritesheet() const = 0;
    virtual std::shared_ptr<const Spritesheet> getHitboxSpritesheet() const = 0;
    virtual std::shared_ptr<const TextureAtlas> getUiTextureAtlas() const = 0;
    virtual std::shared_ptr<const TextureAtlas> getOverlayTextureAtlas() const = 0;
    virtual std::shared_ptr<const Texture> getMenuBackgroundTexture() const = 0;
    virtual std::shared_ptr<const Texture> getTexture(int id) const = 0;
};

/**
 * Interface providing access to audio.
 */
class AudioStore
{
public:
    virtual std::shared_ptr<const WaveFile> getSound(int id) const = 0;
    virtual std::vector<std::shared_ptr<const WaveFile>> getAllSounds() const = 0;
    virtual std::shared_ptr<const MidiFile> getMidi(int id) const = 0;
    virtual const SoundBank* getSoundBank(const std::string& soundId) const = 0;
};

/**
 * Interface providing access to game data.
 */
class DataStore
{
public:
    virtual const UnitDef* getUnitDef(UnitType unitType) const = 0;
    virtual const BuildingDef* getBuildingDef(BuildingType buildingType) const = 0;
    virtual const AttackDef* getAttackDef(int attackId) const = 0;
    virtual const ProjectileDef* getProjectileDef(const std::string& projectileName) const = 0;
};

/**
 * Stores and provides access to all of the game's resources.
 * Where possible, the narrower interface types should be used instead.
 */
class Resources
    : public FontStore
    , public TextureStore
    , public AudioStore
    , public DataStore
{
    friend class ResourceLoader;

public:
    virtual ~Resources() = default;

    // Begin TextureStore override
    std::shared_ptr<const Texture> getPalette() const override;
    std::shared_ptr<const Spritesheet> getTileSpritesheet(bool wilderness) const override;
    std::shared_ptr<const Spritesheet> getUnitSpritesheet(UnitType unitType) const override;
    std::shared_ptr<const Spritesheet> getProjectileSpritesheet(const std::string& name) const override;
    std::shared_ptr<const Spritesheet> getBuildingSpritesheet(BuildingType buildingType) const override;
    std::shared_ptr<const Spritesheet> getCommonObjectSpritesheet() const override;
    std::shared_ptr<const Spritesheet> getObjectSpritesheet(bool wilderness) const override;
    std::shared_ptr<const Spritesheet> getCursorSpritesheet() const override;
    std::shared_ptr<const Spritesheet> getMapBorderSpritesheet() const override;
    std::shared_ptr<const Spritesheet> getPortraitSpritesheet() const override;
    std::shared_ptr<const Spritesheet> getHitboxSpritesheet() const override;
    std::shared_ptr<const TextureAtlas> getUiTextureAtlas() const override;
    std::shared_ptr<const TextureAtlas> getOverlayTextureAtlas() const override;
    std::shared_ptr<const Texture> getMenuBackgroundTexture() const override;
    std::shared_ptr<const Texture> getTexture(int id) const override;
    // End TextureStore override

    // Begin FontStore override
    std::shared_ptr<const Font> getFontSmall() const override;
    std::shared_ptr<const Font> getFontRegular() const override;
    // End FontStore override

    // Begin AudioStore override
    std::shared_ptr<const WaveFile> getSound(int id) const override;
    std::vector<std::shared_ptr<const WaveFile>> getAllSounds() const override;
    std::shared_ptr<const MidiFile> getMidi(int id) const override;
    const SoundBank* getSoundBank(const std::string& soundId) const override;
    // End SoundStore override

    // Begin DataStore override
    const UnitDef* getUnitDef(UnitType unitType) const override;
    const BuildingDef* getBuildingDef(BuildingType buildingType) const override;
    const AttackDef* getAttackDef(int attackId) const override;
    const ProjectileDef* getProjectileDef(const std::string& projectileName) const override;
    // End DataStore override

private:
    // Fonts
    std::shared_ptr<const Font> fontSmall;
    std::shared_ptr<const Font> fontRegular;

    // Loaded textures
    std::vector<std::shared_ptr<const Texture>> textures;
    std::shared_ptr<const Texture> paletteTexture;

    // Spritesheets
    std::unordered_map<UnitType, std::shared_ptr<const Spritesheet>> unitSpritesheets;
    std::unordered_map<BuildingType, std::shared_ptr<const Spritesheet>> buildingSpritesheets;
    std::unordered_map<std::string, std::shared_ptr<const Spritesheet>> projectileSpritesheets;
    std::vector<std::shared_ptr<const Spritesheet>> tileSpritesheets;
    std::vector<std::shared_ptr<const Spritesheet>> objectSpritesheets;
    std::shared_ptr<const Spritesheet> cursorSpritesheet;
    std::shared_ptr<const Spritesheet> mapBorderSpritesheet;
    std::shared_ptr<const Spritesheet> portraitSpritesheet;
    std::shared_ptr<const Spritesheet> hitboxSpritesheet;

    // Texture Atlases
    std::vector<std::shared_ptr<const TextureAtlas>> textureAtlases;

    // Wave Files
    std::vector<std::shared_ptr<const WaveFile>> sounds;

    // MIDI Files
    std::vector<std::shared_ptr<const MidiFile>> midis;

    // Data
    std::unordered_map<UnitType, UnitDef> unitDefs;
    std::unordered_map<BuildingType, BuildingDef> buildingDefs;
    std::unordered_map<int, const AttackDef> attackDefs;
    std::unordered_map<std::string, const ProjectileDef> projectileDefs;
    std::unordered_map<std::string, const EffectDef> effectDefs;
    std::unordered_map<std::string, const SoundBank> soundBanks;
};

}  // namespace Rival
