#pragma once

#include <glm/vec2.hpp>

#include <memory>
#include <unordered_map>

#include "gfx/opengl/renderable/SpriteRenderable.h"
#include "utils/EntityUtils.h"

namespace Rival {

class Camera;
class Entity;
class PlayerStore;
class SpriteComponent;
class Texture;
class TextureStore;
struct PlayerContext;

/**
 * Class responsible for rendering Entities.
 */
class EntityRenderer
{
public:
    EntityRenderer(
            const TextureStore& textureStore, const PlayerContext& playerContext, const PlayerStore& playerStore);

    // Prevent copying
    EntityRenderer(const EntityRenderer&) = delete;
    EntityRenderer& operator=(const EntityRenderer&) = delete;

    void render(const Camera& camera, const EntityContainer& entityContainer, int delta);

    static glm::vec2 getLerpOffset(const Entity& entity, int delta);

private:
    bool isEntityVisible(const Entity& entity, const Camera& camera) const;
    void renderEntity(const Entity& entity, int delta);
    SpriteRenderable* findOrCreateSpriteRenderable(int entityId, const SpriteComponent* spriteComponent);
    bool needsUpdate(const Entity& entity, const SpriteComponent& spriteComponent) const;
    void sendDataToGpu(const Entity& entity,
            const SpriteComponent& spriteComponent,
            const SpriteRenderable& spriteRenderable,
            int delta) const;
    bool shouldRenderHitboxes() const;
    bool isEntityUnderMouse(const Entity& entity) const;
    void renderHitbox(const Entity& entity) const;

private:
    static constexpr int numHitboxSprites = 4;

    std::shared_ptr<const Texture> paletteTexture;
    const PlayerContext& playerContext;
    const PlayerStore& playerStore;

    /** Map of Entity ID -> SpriteRenderable */
    std::unordered_map<int, std::unique_ptr<SpriteRenderable>> entitySpriteRenderables;

    SpriteRenderable hitboxRenderable;
};

}  // namespace Rival
