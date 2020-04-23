#ifndef UNIT_RENDERER_H
#define UNIT_RENDERER_H

#include <glm/mat4x4.hpp>
#include <map>
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

        UnitRenderer(
                std::map<Unit::Type, Spritesheet>& unitSprites,
                Texture& paletteTexture);

        void render(
                Camera& camera,
                std::map<int, std::unique_ptr<Unit>>& units);

    private:

        const std::map<Unit::Type, Spritesheet>& unitSprites;

        const Texture& paletteTexture;

        std::map<int, SpriteRenderable> renderables;

        SpriteRenderable& getOrCreateRenderable(Unit& unit);

        void renderUnit(Unit& unit);

        int getTxIndex(Unit& unit) const;

    };

}

#endif // UNIT_RENDERER_H
