#include "gfx/opengl/renderer/UIRenderer.h"

#include "gfx/opengl/GLWrapper.h"

#include "application/Resources.h"
#include "gfx/Font.h"
#include "gfx/Palette.h"
#include "gfx/PaletteUtils.h"
#include "gfx/RenderUtils.h"
#include "gfx/Texture.h"
#include "gfx/TextureAtlas.h"
#include "gfx/opengl/Shaders.h"
#include "ui/UIRoot.h"
#include "window/Window.h"

namespace Rival {

UIRenderer::UIRenderer(const TextureStore& textureStore, const Window* window)
    : window(window)
    , paletteTexture(textureStore.getPalette())
{
}

void UIRenderer::render(const UIRoot& uiRoot)
{
    // Disable depth testing since we are drawing in tree order
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, window->getWidth(), window->getHeight());

    setUpShaders();

    drawnElementIds.clear();
    renderElement(uiRoot, { 0, 0 });
    cleanUpStaleEntries();
}

void UIRenderer::setUpShaders() const
{
    const glm::mat4 viewProjMatrix = RenderUtils::createMenuViewProjectionMatrix(window->getAspectRatio());

    glUseProgram(Shaders::boxShader.programId);
    glUniformMatrix4fv(Shaders::boxShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);

    glUseProgram(Shaders::indexedTextureShader.programId);
    glUniformMatrix4fv(Shaders::indexedTextureShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);
    glUniform1i(Shaders::indexedTextureShader.texUnitUniformLoc, 0);
    glUniform1i(Shaders::indexedTextureShader.paletteTexUnitUniformLoc, 1);
    glUniform1f(Shaders::indexedTextureShader.transparentIndex, Palette::globalTransparentColor);

    glUseProgram(Shaders::fontShader.programId);
    glUniformMatrix4fv(Shaders::fontShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);
    glUniform1i(Shaders::fontShader.texUnitUniformLoc, 0);

    // The palette is the only texture shared by all images
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, paletteTexture->getId());
}

void UIRenderer::renderElement(const UIElement& element, const glm::ivec2& parentPosition)
{
    const glm::ivec2 position = parentPosition + element.getPosition();

    if (const UIVisual* visual = element.getVisual())
    {
        renderVisual(element.getId(), *visual, position, element.getSize());
    }

    for (const auto& child : element.getAllChildren())
    {
        renderElement(*child, position);
    }
}

void UIRenderer::renderVisual(int elementId, const UIVisual& visual, const glm::ivec2& position, const glm::ivec2& size)
{
    drawnElementIds.insert(elementId);

    auto it = entries.find(elementId);
    bool needsUpload = false;

    // Create a new renderable if this element has never been drawn.
    // NOTE: It is assumed that an element's visual type never changes.
    if (it == entries.end())
    {
        RenderData newEntry { visual, position, size, createRenderable(visual) };
        it = entries.insert_or_assign(elementId, std::move(newEntry)).first;
        needsUpload = true;
    }

    RenderData& entry = it->second;
    if (entry.visual != visual || entry.position != position || entry.size != size)
    {
        entry.visual = visual;
        entry.position = position;
        entry.size = size;
        needsUpload = true;
    }

    if (const BoxVisual* box = std::get_if<BoxVisual>(&visual))
    {
        renderBox(*box, entry, needsUpload);
    }
    else if (const ImageVisual* image = std::get_if<ImageVisual>(&visual))
    {
        renderImage(*image, entry, needsUpload);
    }
    else if (const TextVisual* text = std::get_if<TextVisual>(&visual))
    {
        renderText(*text, entry, needsUpload);
    }
}

UIRenderer::Renderable UIRenderer::createRenderable(const UIVisual& visual)
{
    if (const ImageVisual* image = std::get_if<ImageVisual>(&visual))
    {
        return std::make_unique<AtlasRenderable>(image->atlas, 1);
    }

    if (std::holds_alternative<TextVisual>(visual))
    {
        return std::make_unique<TextRenderable>();
    }

    return std::make_unique<BoxRenderable>(1);
}

void UIRenderer::renderBox(const BoxVisual& box, RenderData& entry, bool needsUpload)
{
    BoxRenderable& renderable = *std::get<std::unique_ptr<BoxRenderable>>(entry.renderable);

    if (needsUpload)
    {
        const glm::vec2 topLeft = entry.position;
        const glm::vec2 bottomRight = entry.position + entry.size;
        renderable.setBoxes({ { topLeft, bottomRight, box.color } });
    }

    glUseProgram(Shaders::boxShader.programId);
    renderable.draw();
}

void UIRenderer::renderImage(const ImageVisual& image, RenderData& entry, bool needsUpload)
{
    AtlasRenderable& renderable = *std::get<std::unique_ptr<AtlasRenderable>>(entry.renderable);

    if (needsUpload)
    {
        renderable.setImages({ { entry.position, entry.size, image.imageKey } });
    }

    glUseProgram(Shaders::indexedTextureShader.programId);
    glUniform1f(Shaders::indexedTextureShader.paletteTxYUnitUniformLoc,
            PaletteUtils::getPaletteTxY(image.atlas->getPaletteIndex()));
    renderable.draw();
}

void UIRenderer::renderText(const TextVisual& text, RenderData& entry, bool needsUpload)
{
    TextRenderable& renderable = *std::get<std::unique_ptr<TextRenderable>>(entry.renderable);

    if (needsUpload)
    {
        const float x = static_cast<float>(entry.position.x);
        const float baselineY = entry.position.y + text.properties.font->getAscent(text.properties.scale);
        renderable.setText({ text.span }, text.properties, x, baselineY);
    }

    glUseProgram(Shaders::fontShader.programId);
    renderable.draw();
}

void UIRenderer::cleanUpStaleEntries()
{
    // Any element that wasn't drawn this frame has been removed (or has lost its visual)
    std::erase_if(entries, [this](const auto& pair) { return !drawnElementIds.contains(pair.first); });
}

}  // namespace Rival
