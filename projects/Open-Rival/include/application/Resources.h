#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "audio/MidiFile.h"
#include "audio/WaveFile.h"
#include "game/BuildingDef.h"
#include "game/BuildingType.h"
#include "game/UnitDef.h"
#include "game/UnitType.h"
#include "gfx/Font.h"
#include "gfx/PaletteUtils.h"
#include "gfx/Spritesheet.h"
#include "gfx/Texture.h"
#include "gfx/TextureAtlas.h"

namespace Rival {

class ApplicationContext;

/**
 * Interface providing access to fonts.
 */
class FontStore
{
public:
    virtual const Font& getFontSmall() const = 0;
    virtual const Font& getFontRegular() const = 0;
};

/**
 * Interface providing access to textures.
 */
class TextureStore
{
public:
    virtual std::shared_ptr<const Texture> getPalette() const = 0;
    virtual const Spritesheet& getTileSpritesheet(bool wilderness) const = 0;
    virtual const Spritesheet& getUnitSpritesheet(UnitType unitType) const = 0;
    virtual const Spritesheet& getBuildingSpritesheet(BuildingType buildingType) const = 0;
    virtual const Spritesheet& getCommonObjectSpritesheet() const = 0;
    virtual const Spritesheet& getObjectSpritesheet(bool wilderness) const = 0;
    virtual const Spritesheet& getCursorSpritesheet() const = 0;
    virtual const Spritesheet& getMapBorderSpritesheet() const = 0;
    virtual const Spritesheet& getPortraitSpritesheet() const = 0;
    virtual const Spritesheet& getHitboxSpritesheet() const = 0;
    virtual std::shared_ptr<const TextureAtlas> getUiTextureAtlas() const = 0;
    virtual std::shared_ptr<const TextureAtlas> getOverlayTextureAtlas() const = 0;
    virtual std::shared_ptr<const Texture> getMenuBackgroundTexture() const = 0;
};

/**
 * Interface providing access to audio.
 */
class AudioStore
{
public:
    virtual const WaveFile& getSound(int id) const = 0;
    virtual const MidiFile& getMidi(int id) const = 0;
};

/**
 * Interface providing access to game data.
 */
class DataStore
{
public:
    virtual const UnitDef* getUnitDef(UnitType unitType) const = 0;
    virtual const BuildingDef* getBuildingDef(BuildingType buildingType) const = 0;
};

/**
 * Class that holds all of the game's resources.
 */
class Resources
    : public FontStore
    , public TextureStore
    , public AudioStore
    , public DataStore
{
public:
    Resources(ApplicationContext& context);
    virtual ~Resources() = default;

    // Begin TextureStore override
    std::shared_ptr<const Texture> getPalette() const override;
    const Spritesheet& getTileSpritesheet(bool wilderness) const override;
    const Spritesheet& getUnitSpritesheet(UnitType unitType) const override;
    const Spritesheet& getBuildingSpritesheet(BuildingType buildingType) const override;
    const Spritesheet& getCommonObjectSpritesheet() const override;
    const Spritesheet& getObjectSpritesheet(bool wilderness) const override;
    const Spritesheet& getCursorSpritesheet() const override;
    const Spritesheet& getMapBorderSpritesheet() const override;
    const Spritesheet& getPortraitSpritesheet() const override;
    const Spritesheet& getHitboxSpritesheet() const override;
    std::shared_ptr<const TextureAtlas> getUiTextureAtlas() const override;
    std::shared_ptr<const TextureAtlas> getOverlayTextureAtlas() const override;
    std::shared_ptr<const Texture> getMenuBackgroundTexture() const override;
    // End TextureStore override

    // Begin FontStore override
    const Font& getFontSmall() const override;
    const Font& getFontRegular() const override;
    // End FontStore override

    // Begin SoundStore override
    const WaveFile& getSound(int id) const override;
    const MidiFile& getMidi(int id) const override;
    // End SoundStore override

    // Begin DataStore override
    const UnitDef* getUnitDef(UnitType unitType) const override;
    const BuildingDef* getBuildingDef(BuildingType buildingType) const override;
    // End DataStore override

private:
    // Initialization
    Font initFontSmall();
    Font initFontRegular();
    std::vector<std::shared_ptr<const Texture>> loadTextures();
    std::vector<std::shared_ptr<const TextureAtlas>> loadTextureAtlases();
    std::shared_ptr<const Texture> initPaletteTexture();
    std::unordered_map<BuildingType, Spritesheet> initBuildingSpritesheets();
    std::unordered_map<UnitType, Spritesheet> initUnitSpritesheets();
    std::vector<Spritesheet> initTileSpritesheets();
    std::vector<Spritesheet> initObjectSpritesheets();
    Spritesheet initCursorSpritesheet();
    Spritesheet initMapBorderSpritesheet();
    Spritesheet initPortraitSpritesheet();
    Spritesheet initHitboxSpritesheet();
    std::vector<WaveFile> initSounds();
    std::vector<MidiFile> initMidis();
    std::unordered_map<UnitType, UnitDef> initUnitDefs() const;
    std::unordered_map<BuildingType, BuildingDef> initBuildingDefs() const;

public:
    // Directories
    static const std::string dataDir;
    static const std::string mapsDir;
    static const std::string soundDir;
    static const std::string txDir;

private:
    // Font settings
    static const std::vector<std::string> defaultFontDirs;
    static const std::string defaultFontSmall;
    static const std::string defaultFontRegular;

    // Texture constants
    static constexpr int txIndexUnits = 0;
    static constexpr int txIndexTiles = 50;
    static constexpr int txIndexObjects = 54;
    static constexpr int txIndexCursors = 57;
    static constexpr int txIndexBuildings = 58;
    static constexpr int txIndexPortraits = 61;
    static constexpr int txIndexHitbox = 62;
    static constexpr int txIndexMenuBackground = 63;

    // Resource counts
    static constexpr int numTextures = txIndexHitbox + 1;
    static constexpr int numTextureAtlases = 1;
    static constexpr int numSounds = 369;
    static constexpr int numMidis = 1;  // 13;

    // MIDI constants
    static constexpr int midiStartIndex = 369;

    ApplicationContext& context;

    // Fonts
    Font fontSmall;
    Font fontRegular;

    // Loaded textures
    std::vector<std::shared_ptr<const Texture>> textures;
    std::shared_ptr<const Texture> paletteTexture;

    // Spritesheets
    std::unordered_map<UnitType, Spritesheet> unitSpritesheets;
    std::unordered_map<BuildingType, Spritesheet> buildingSpritesheets;
    std::vector<Spritesheet> tileSpritesheets;
    std::vector<Spritesheet> objectSpritesheets;
    Spritesheet cursorSpritesheet;
    Spritesheet mapBorderSpritesheet;
    Spritesheet portraitSpritesheet;
    Spritesheet hitboxSpritesheet;

    // Texture Atlases
    std::vector<std::shared_ptr<const TextureAtlas>> textureAtlases;

    // Wave Files
    std::vector<WaveFile> sounds;

    // MIDI Files
    std::vector<MidiFile> midis;

    // Data
    std::unordered_map<UnitType, UnitDef> unitDefs;
    std::unordered_map<BuildingType, BuildingDef> buildingDefs;
};

}  // namespace Rival
