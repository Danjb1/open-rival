#ifndef UNIT_RENDERER_H
#define UNIT_RENDERER_H

#include <map>

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

        UnitRenderer(Texture& paletteTexture);

        void render(std::map<int, std::unique_ptr<Unit>>& units);

    private:

        const Texture& paletteTexture;

        std::map<int, Renderable> renderables;

        Renderable& getOrCreateRenderable(
                const std::unique_ptr<Unit>& unit);

    };

}

#endif // UNIT_RENDERER_H
