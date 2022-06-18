#pragma once

#include <ft2build.h>

#include <string>
#include <unordered_map>
#include <vector>
#include FT_FREETYPE_H

#include "Building.h"
#include "Font.h"
#include "MidiFile.h"
#include "PaletteUtils.h"
#include "Spritesheet.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "Unit.h"
#include "WaveFile.h"
#include "json.h"

using json = nlohmann::json;

namespace Rival {

class Resources
{

public:
    // Directories
    static const std::string dataDir;
    static const std::string mapsDir;
    static const std::string soundDir;
    static const std::string txDir;

    Resources(json& cfg);
    ~Resources();

    // Prevent moving or copying (rule of 5)
    Resources(const Resources& other) = delete;
    Resources(Resources&& other) = delete;
    Resources& operator=(const Resources& other) = delete;
    Resources& operator=(Resources&& other) = delete;

    // Retrieval
    const Font& getFontSmall() const;
    const Font& getFontRegular() const;
    const Texture& getPalette() const;
    const Spritesheet& getTileSpritesheet(bool wilderness) const;
    const Spritesheet& getUnitSpritesheet(Unit::Type unitType) const;
    const Spritesheet& getBuildingSpritesheet(Building::Type buildingType) const;
    const Spritesheet& getCommonObjectSpritesheet() const;
    const Spritesheet& getObjectSpritesheet(bool wilderness) const;
    const Spritesheet& getMapBorderSpritesheet() const;
    const TextureAtlas& getUiTextureAtlas() const;
    const WaveFile& getSound(int id) const;
    const MidiFile& getMidi(int id) const;

private:
    // Font settings
    static const std::vector<std::string> defaultFontDirs;
    static const std::string defaultFontSmall;
    static const std::string defaultFontRegular;

    // Resource counts
    static constexpr int numTextures = 58;
    static constexpr int numTextureAtlases = 1;
    static constexpr int numSounds = 369;
    static constexpr int numMidis = 1;  // 13;

    // Texture constants
    static constexpr int txIndexUnits = 0;
    static constexpr int txIndexTiles = 50;
    static constexpr int txIndexObjects = 54;
    static constexpr int txIndexCursors = 57;
    static constexpr int txIndexBuildings = 58;

    // MIDI constants
    static constexpr int midiStartIndex = 369;

    json& cfg;

    // Fonts
    FT_Library freeTypeLib;
    Font fontSmall;
    Font fontRegular;

    // Loaded textures
    std::vector<Texture> textures;
    Texture paletteTexture;

    // Spritesheets
    std::unordered_map<Unit::Type, Spritesheet> unitSpritesheets;
    std::unordered_map<Building::Type, Spritesheet> buildingSpritesheets;
    std::vector<Spritesheet> tileSpritesheets;
    std::vector<Spritesheet> objectSpritesheets;
    Spritesheet mapBorderSpritesheet;

    // Texture Atlases
    std::vector<TextureAtlas> textureAtlases;

    // Wave Files
    std::vector<WaveFile> sounds;

    // MIDI Files
    std::vector<MidiFile> midis;

    // Initialization
    FT_Library initFreeType();
    Font initFontSmall();
    Font initFontRegular();
    std::vector<Texture> loadTextures();
    std::vector<TextureAtlas> loadTextureAtlases();
    Texture initPaletteTexture();
    std::unordered_map<Building::Type, Spritesheet> initBuildingSpritesheets();
    std::unordered_map<Unit::Type, Spritesheet> initUnitSpritesheets();
    std::vector<Spritesheet> initTileSpritesheets();
    std::vector<Spritesheet> initObjectSpritesheets();
    Spritesheet initMapBorderSpritesheet();
    std::vector<WaveFile> initSounds();
    std::vector<MidiFile> initMidis();
};

}  // namespace Rival
