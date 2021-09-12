#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H

#include <array>
#include <map>
#include <string>

#include "Camera.h"
#include "Entity.h"
#include "SpriteComponent.h"
#include "Texture.h"

namespace Rival {

    /**
     * Class responsible for rendering Entities.
     */
    class EntityRenderer {

    public:
        EntityRenderer(const Texture& paletteTexture);

        // Prevent copying
        EntityRenderer(const EntityRenderer&) = delete;
        EntityRenderer& operator=(const EntityRenderer&) = delete;

        void render(
                const Camera& camera,
                const std::vector<std::shared_ptr<Entity>> entities,
                int delta) const;

    private:
        static const int numLerpDimensions = 2;
        static const int lerpIdxX = 0;
        static const int lerpIdxY = 1;

        const Texture& paletteTexture;

        bool isEntityVisible(
                const Entity& entity,
                const Camera& camera) const;

        void renderEntity(Entity& entity, int delta) const;

        bool needsUpdate(
                const Entity& entity,
                const SpriteComponent* spriteComponent) const;

        void sendDataToGpu(
                const Entity& entity,
                const SpriteComponent* spriteComponent,
                int delta) const;

        std::array<float, numLerpDimensions> getLerpOffset(
                const Entity& entity, int delta) const;
    };

}  // namespace Rival

#endif  // ENTITY_RENDERER_H
