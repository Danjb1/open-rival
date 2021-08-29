#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>
#include <string>
#include <vector>

#include <ft2build.h>
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

namespace Rival {

    class Resources {

    public:
        // Directories
        static const std::string fontDir;
        static const std::string mapsDir;
        static const std::string soundDir;
        static const std::string txDir;

        Resources();
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
        // Resource counts
        static const int numTextures = 58;
        static const int numTextureAtlases = 1;
        static const int numSounds = 369;
        static const int numMidis = 1;  //13;

        // Texture constants
        static const int txIndexUnits = 0;
        static const int txIndexTiles = 50;
        static const int txIndexObjects = 54;
        static const int txIndexCursors = 57;
        static const int txIndexBuildings = 58;

        // MIDI constants
        static const int midiStartIndex = 369;

        // Fonts
        FT_Library freeTypeLib;
        Font fontSmall;
        Font fontRegular;

        // Loaded textures
        std::vector<Texture> textures;
        Texture paletteTexture;

        // Spritesheets
        std::map<Unit::Type, Spritesheet> unitSpritesheets;
        std::map<Building::Type, Spritesheet> buildingSpritesheets;
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
        std::map<Building::Type, Spritesheet> initBuildingSpritesheets();
        std::map<Unit::Type, Spritesheet> initUnitSpritesheets();
        std::vector<Spritesheet> initTileSpritesheets();
        std::vector<Spritesheet> initObjectSpritesheets();
        Spritesheet initMapBorderSpritesheet();
        std::vector<WaveFile> initSounds();
        std::vector<MidiFile> initMidis();
    };

}  // namespace Rival

#endif  // RESOURCES_H
