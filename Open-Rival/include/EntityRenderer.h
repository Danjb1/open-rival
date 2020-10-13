#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H

#include <map>
#include <string>

#include "Camera.h"
#include "Entity.h"
#include "SpriteComponent.h"
#include "Texture.h"

namespace Rival {

    /**
     * Class responsible for rendering Entities.
     *
     * Entities must register themselves with the Renderer when spawned, and
     * unregister themselves when they are removed.
     */
    class EntityRenderer {

    public:
        EntityRenderer(const Texture& paletteTexture);

        // Prevent copying
        EntityRenderer(const EntityRenderer&) = delete;
        EntityRenderer& operator=(const EntityRenderer&) = delete;

        void render(
                const Camera& camera,
                const std::map<int, std::unique_ptr<Entity>>&) const;

    private:
        const Texture& paletteTexture;

        bool isEntityVisible(
                const Entity& entity,
                const Camera& camera) const;

        void renderEntity(Entity& entity) const;

        bool needsUpdate(
                const Entity& entity,
                const SpriteComponent* spriteComponent) const;

        void sendDataToGpu(
                Entity& entity,
                SpriteComponent* spriteComponent) const;
    };

}  // namespace Rival

#endif  // ENTITY_RENDERER_H
