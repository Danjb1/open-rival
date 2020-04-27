#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>
#include <string>
#include <vector>

#include "Palette.h"
#include "Spritesheet.h"
#include "Texture.h"
#include "Unit.h"

namespace Rival {

    class Resources {

    public:

        // Directories
        static const std::string mapsDir;
        static const std::string txDir;

        Resources();
        ~Resources();

        // Retrieval
        Texture& getPalette();
        Spritesheet& getTileSpritesheet(int index);
        std::map<UnitType, Spritesheet>& getUnitSpritesheets();
        Spritesheet& getMapBorderSpritesheet();

    private:

        // Texture constants
        static const int numTextures = 96;
        static const int txIndexUnits = 0;
        static const int txIndexTiles = 50;
        static const int txIndexUi = 53;

        // Loaded textures
        std::unique_ptr<std::vector<Texture>> textures =
            std::make_unique<std::vector<Texture>>();
        std::unique_ptr<Texture> paletteTexture;

        // Spritesheets
        std::unique_ptr<std::map<UnitType, Spritesheet>> unitSpritesheets =
            std::make_unique<std::map<UnitType, Spritesheet>>();
        std::unique_ptr<std::map<int, Spritesheet>> tileSpritesheets =
            std::make_unique<std::map<int, Spritesheet>>();
        std::unique_ptr<Spritesheet> mapBorderSpritesheet;

        // Initialisation
        void loadTextures();
        void initPaletteTexture();
        void initUnitSpritesheets();
        void initTileSpritesheets();
        void initUiSpritesheets();
        void initUnitSpritesheet(UnitType type, int txIndex);
        void initTileSpritesheet(int type, int txIndex);

    };

}

#endif // TIMER_UTILS_H
