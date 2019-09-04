#ifndef UNIT_RENDERER_H
#define UNIT_RENDERER_H

#include <glm/mat4x4.hpp>
#include <map>
#include <string>

#include "ImageRenderable.h"
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
                std::map<Unit::Type, Sprite>& unitSprites,
                Texture& paletteTexture);

        void render(
                glm::mat4 viewProjMatrix,
                std::map<int, std::unique_ptr<Unit>>& units);

    private:

        const std::map<Unit::Type, Sprite>& unitSprites;

        const Texture& paletteTexture;

        std::map<int, ImageRenderable> renderables;

        ImageRenderable& getOrCreateRenderable(
                const std::unique_ptr<Unit>& unit);

        int getUnitRenderPosX(Unit& unit);

        int getUnitRenderPosY(Unit& unit);

    };

}

#endif // UNIT_RENDERER_H
