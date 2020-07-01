#ifndef UNIT_RENDERER_H
#define UNIT_RENDERER_H

#include <map>
#include <memory>
#include <string>

#include "Camera.h"
#include "Spritesheet.h"
#include "SpriteRenderable.h"
#include "Unit.h"

namespace Rival {

    /**
     * Class responsible for rendering Units.
     *
     * It creates a Renderable for each Unit, and holds onto the reference
     * until that Unit is deleted.
     */
    class UnitRenderer {

    public:
        // Offset applied to align a unit sprite correctly within a tile,
        // in pixels
        static const int unitOffsetX = -33;
        static const int unitOffsetY = -89;

        UnitRenderer(
                const std::map<UnitType, Spritesheet>& unitSprites,
                const Texture& paletteTexture);

        UnitRenderer(const UnitRenderer&) = delete;
        UnitRenderer& operator=(const UnitRenderer&) = delete;

        void render(
                const Camera& camera,
                const std::map<int, std::unique_ptr<Unit>>& units);

    private:
        const std::map<UnitType, Spritesheet>& unitSprites;

        const Texture& paletteTexture;

        std::map<int, SpriteRenderable> renderables;

        SpriteRenderable& getOrCreateRenderable(const Unit& unit);

        void renderUnit(const Unit& unit);

        bool isUnitVisible(const Unit& unit, const Camera& camera);

        int getTxIndex(const Unit& unit) const;
    };

}  // namespace Rival

#endif  // UNIT_RENDERER_H
