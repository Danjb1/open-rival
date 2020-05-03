#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Building.h"
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

        // Prevent moving or copying (rule of 5)
        Resources(const Resources& other) = delete;
        Resources(Resources&& other) = delete;
        Resources& operator=(const Resources& other) = delete;
        Resources& operator=(Resources&& other) = delete;

        // Retrieval
        Texture& getPalette() const;
        Spritesheet& getTileSpritesheet(int index) const;
        std::map<UnitType, Spritesheet>& getUnitSpritesheets() const;
        std::map<BuildingType, Spritesheet>& getBuildingSpritesheets() const;
        Spritesheet& getMapBorderSpritesheet() const;

    private:

        // Texture constants
        static const int numTextures = 96;
        static const int txIndexUnits = 0;
        static const int txIndexTiles = 50;
        static const int txIndexUi = 53;
        static const int txIndexBuildings = 55;

        // Loaded textures
        std::unique_ptr<std::vector<Texture>> textures;
        std::unique_ptr<Texture> paletteTexture;

        // Spritesheets
        std::unique_ptr<std::map<UnitType, Spritesheet>> unitSpritesheets;
        std::unique_ptr<std::map<BuildingType, Spritesheet>> buildingSpritesheets;
        std::unique_ptr<std::map<int, Spritesheet>> tileSpritesheets;
        std::unique_ptr<Spritesheet> mapBorderSpritesheet;

        // Initialisation
        void loadTextures();
        void initPaletteTexture();
        void initBuildingSpritesheets();
        void initUnitSpritesheets();
        void initTileSpritesheets();
        void initUiSpritesheets();

    };

}

#endif // RESOURCES_H
