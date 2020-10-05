#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>
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
        const Texture& getPalette() const;
        const Spritesheet& getTileSpritesheet(bool wilderness) const;
        const Spritesheet& getUnitSpritesheet(Unit::Type unitType) const;
        const Spritesheet& getBuildingSpritesheet(Building::Type buildingType) const;
        const Spritesheet& getMapBorderSpritesheet() const;

    private:
        // Texture constants
        static const int numTextures = 96;
        static const int txIndexUnits = 0;
        static const int txIndexTiles = 50;
        static const int txIndexCursors = 54;
        static const int txIndexBuildings = 55;

        // Loaded textures
        const std::vector<Texture> textures;
        const Texture paletteTexture;

        // Spritesheets
        const std::map<Unit::Type, Spritesheet> unitSpritesheets;
        const std::map<Building::Type, Spritesheet> buildingSpritesheets;
        const std::map<int, Spritesheet> tileSpritesheets;
        const Spritesheet mapBorderSpritesheet;

        // Initialisation
        std::vector<Texture> loadTextures();
        Texture initPaletteTexture();
        std::map<Building::Type, Spritesheet> initBuildingSpritesheets();
        std::map<Unit::Type, Spritesheet> initUnitSpritesheets();
        std::map<int, Spritesheet> initTileSpritesheets();
        Spritesheet initMapBorderSpritesheet();
    };

}  // namespace Rival

#endif  // RESOURCES_H
