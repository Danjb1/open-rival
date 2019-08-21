#ifndef UNIT_RENDERER_H
#define UNIT_RENDERER_H

#include <map>

#include "Unit.h"

namespace Rival {

    /**
     * Class responsible for rendering Units.
     */
    class UnitRenderer {

    public:

        UnitRenderer(Texture& paletteTexture);

        void render(std::map<int, std::unique_ptr<Unit>>& units) const;

    private:

        const Texture& paletteTexture;

    };

}

#endif // UNIT_RENDERER_H
