#include "gfx/opengl/renderer/EntityOverlayRenderer.h"

#include "application/Resources.h"
#include "entity/Entity.h"
#include "entity/Unit.h"
#include "entity/components/HealthComponent.h"
#include "entity/components/MouseHandlerComponent.h"
#include "entity/components/OwnerComponent.h"
#include "entity/components/SpriteComponent.h"
#include "game/PlayerContext.h"
#include "gfx/Camera.h"
#include "gfx/Palette.h"
#include "gfx/PaletteUtils.h"
#include "gfx/RenderUtils.h"
#include "gfx/opengl/Shaders.h"
#include "utils/CollectionUtils.h"
#include "utils/LogUtils.h"
#include "utils/MathUtils.h"

namespace Rival {

const std::string EntityOverlayRenderer::healthBarAtlasKey = "img_overlay_health.tga";
const std::string EntityOverlayRenderer::healthBarDepletedAtlasKey = "img_overlay_health_depleted.tga";
const std::string EntityOverlayRenderer::monsterHealthBarAtlasKey = "img_overlay_health_monster.tga";
const std::string EntityOverlayRenderer::monsterHealthBarDepletedAtlasKey = "img_overlay_health_monster_depleted.tga";

EntityOverlayRenderer::EntityOverlayRenderer(const TextureStore& textureStore, const PlayerContext& playerContext)
    : playerContext(playerContext)
    , paletteTexture(textureStore.getPalette())
    , overlayRenderable(textureStore.getOverlayTextureAtlas(), maxImagesToRender)
    , boxRenderable(maxOverlaysToRender)
    , backgroundColor(PaletteUtils::fromHex(Palette::paletteGame[212]))
{
    boxes.reserve(maxOverlaysToRender);
    images.reserve(maxImagesToRender);
}

void EntityOverlayRenderer::prepare(const EntityContainer& entityContainer)
{
    // Reset
    boxes.clear();
    images.clear();

    const auto hoveredEntity = playerContext.weakEntityUnderMouse.lock();
    const std::set<int> selectedEntityIds = playerContext.getSelectedEntityIds();

    // Fill buffers
    entityContainer.forEachEntity([&](const auto& entity) {
        if (!entity->isVisible())
        {
            // Don't render overlays for invisible Entities
            return;
        }

        const bool isHovered = (entity == hoveredEntity);
        const bool isSelected = selectedEntityIds.contains(entity->getId());
        if (!isHovered && !isSelected)
        {
            return;
        }

        if (const Unit* unit = entity->as<Unit>())
        {
            // Special case for Units:
            // do not render the overlay when they are playing their death animation
            if (unit->getState() == UnitState::Dying)
            {
                return;
            }
        }

        addEntityOverlayToBuffers(*entity, isHovered);
    });
}

void EntityOverlayRenderer::renderBoxes()
{
    boxRenderable.setBoxes(boxes);
    boxRenderable.draw();
}

void EntityOverlayRenderer::renderTextures()
{
    // Use the palette of the overlay images
    const int paletteIndex = overlayRenderable.texAtlas->getPaletteIndex();
    glUniform1f(Shaders::indexedTextureShader.paletteTxYUnitUniformLoc, PaletteUtils::getPaletteTxY(paletteIndex));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, paletteTexture->getId());

    overlayRenderable.setImages(images);
    overlayRenderable.draw();
}

// TODO: This should be a common utility function
bool isMonster(const Entity& entity)
{
    if (const Unit* unit = entity.as<Unit>())
    {
        // Monsters have no owner
        return unit->getComponent<OwnerComponent>(OwnerComponent::key) == nullptr;
    }

    return false;
}

// TODO: Hovered entities show more info, e.g. experience level, bigger icons, and upgrades
void EntityOverlayRenderer::addEntityOverlayToBuffers(const Entity& entity, bool /* isHovered */)
{
    if (static_cast<int>(images.size()) + maxSpritesPerEntity >= maxImagesToRender
            || static_cast<int>(boxes.size()) + 1 >= maxOverlaysToRender)
    {
        // Don't overflow the buffer
        return;
    }

    const HealthComponent* healthComponent = entity.getComponent<HealthComponent>();
    if (!healthComponent)
    {
        return;
    }

    const SpriteComponent* spriteComponent = entity.getComponent<SpriteComponent>();
    if (!spriteComponent)
    {
        return;
    }

    const MouseHandlerComponent* mouseHandlerComponent = entity.getComponent<MouseHandlerComponent>();
    if (!mouseHandlerComponent)
    {
        return;
    }

    const Rect& hitbox = mouseHandlerComponent->getHitbox();

    const MapNode& pos = entity.getPos();
    const float tileX = static_cast<float>(RenderUtils::tileToPx_X(pos.x));
    const float tileY = static_cast<float>(RenderUtils::tileToPx_Y(pos.x, pos.y));

    // The overlay is always rendered a fixed distance above a unit's hitbox
    const float originX = tileX + healthBarDrawOffsetX + spriteComponent->lastLerpOffset.x;
    float originY = tileY + (-hitbox.height) + healthBarDrawOffsetY + spriteComponent->lastLerpOffset.y;

    // Add a y-offset for flying Units
    if (const Unit* unit = entity.as<Unit>())
    {
        originY += unit->getOffsetY();
    }

    ////////////////
    // Background //
    ////////////////

    const glm::vec2 backgroundTopLeft { originX, originY };
    const glm::vec2 backgroundSize { healthBarWidth, 2 * healthBarHeight };
    boxes.push_back({ backgroundTopLeft, backgroundTopLeft + backgroundSize, backgroundColor });

    const bool bIsMonster = isMonster(entity);
    const std::string currentHealthBarAtlasKey = bIsMonster ? monsterHealthBarAtlasKey : healthBarAtlasKey;
    const std::string currentDepletedHealthBarAtlasKey =
            bIsMonster ? monsterHealthBarDepletedAtlasKey : healthBarDepletedAtlasKey;

    //////////////////
    // Health bar 1 //
    //////////////////

    {
        const float x1 = originX;
        const float y1 = originY;

        // Depleted
        if (healthComponent->getHealth() < healthPerBar)
        {
            const float widthRatio =
                    MathUtils::clampf(healthComponent->getMaxHealth() / static_cast<float>(healthPerBar), 0.f, 1.f);

            const glm::vec2 size { healthBarWidth * widthRatio, healthBarHeight };
            images.push_back({ { x1, y1 }, size, currentDepletedHealthBarAtlasKey });
        }

        // Remaining
        {
            const float widthRatio =
                    MathUtils::clampf(healthComponent->getHealth() / static_cast<float>(healthPerBar), 0.f, 1.f);

            const glm::vec2 size { healthBarWidth * widthRatio, healthBarHeight };
            images.push_back({ { x1, y1 }, size, currentHealthBarAtlasKey });
        }
    }

    //////////////////
    // Health bar 2 //
    //////////////////

    const bool hasSecondBar = healthComponent->getMaxHealth() > healthPerBar;
    if (hasSecondBar)
    {
        const float x1 = originX;
        const float y1 = originY + healthBarHeight;

        // Depleted
        if (healthComponent->getHealth() < 2 * healthPerBar)
        {
            const int maxHealthThisBar = healthComponent->getMaxHealth() - healthPerBar;
            const float widthRatio = MathUtils::clampf(maxHealthThisBar / static_cast<float>(healthPerBar), 0.f, 1.f);

            const glm::vec2 size { healthBarWidth * widthRatio, healthBarHeight };
            images.push_back({ { x1, y1 }, size, currentDepletedHealthBarAtlasKey });
        }

        // Remaining
        {
            const int healthThisBar = healthComponent->getHealth() - healthPerBar;
            const float widthRatio = MathUtils::clampf(healthThisBar / static_cast<float>(healthPerBar), 0.f, 1.f);

            const glm::vec2 size { healthBarWidth * widthRatio, healthBarHeight };
            images.push_back({ { x1, y1 }, size, currentHealthBarAtlasKey });
        }
    }
}

}  // namespace Rival
