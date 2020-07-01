#ifndef BUILDING_RENDERER_H
#define BUILDING_RENDERER_H

#include <map>
#include <memory>
#include <string>

#include "Building.h"
#include "Camera.h"
#include "Spritesheet.h"
#include "SpriteRenderable.h"

namespace Rival {

    /**
     * Class responsible for rendering Buildings.
     *
     * It creates a Renderable for each Building, and holds onto the reference
     * until that Building is deleted.
     */
    class BuildingRenderer {

    public:
        // Offset applied to align a building sprite correctly within a tile,
        // in pixels
        static const int buildingOffsetX = -33;
        static const int buildingOffsetY = -89;

        BuildingRenderer(
                const std::map<BuildingType, Spritesheet>& buildingSprites,
                const Texture& paletteTexture);

        BuildingRenderer(const BuildingRenderer&) = delete;
        BuildingRenderer& operator=(const BuildingRenderer&) = delete;

        void render(
                const Camera& camera,
                const std::map<int, std::unique_ptr<Building>>& buildings);

    private:
        const std::map<BuildingType, Spritesheet>& buildingSprites;

        const Texture& paletteTexture;

        std::map<int, SpriteRenderable> renderables;

        SpriteRenderable& getOrCreateRenderable(const Building& building);

        void renderBuilding(const Building& building);

        bool isBuildingVisible(const Building& building, const Camera& camera);

        int getTxIndex(const Building& building) const;
    };

}  // namespace Rival

#endif  // BUILDING_RENDERER_H
