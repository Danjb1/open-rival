#include "gfx/opengl/renderer/LobbyRenderer.h"

#include "lobby/LobbyState.h"

namespace Rival {

Rect makeViewport(const Window* window)
{
    // Fill the screen
    return { 0, 0, window->getWidth(), window->getHeight() };
}

LobbyRenderer::LobbyRenderer(LobbyState& lobby, const Window* window)
    : lobby(lobby)
    , res(lobby.getResources())
    , menuRenderer(res, window, makeViewport(window))
    , textRenderer(window)
{
}

void LobbyRenderer::render(int delta)
{
    menuRenderer.render(delta);
    renderText();
}

void LobbyRenderer::renderText()
{
    // TMP: Hardcoded hacky rendering until we have a proper menu system.
    // The TextRenderables should be long-lived objects - we should not be recreating them every frame.
    std::vector<TextRenderable> textRenderables;
    TextProperties nameProperties = { res.getFontRegular() };
    glm::vec2 renderPos = { 100, 100 };
    const float rowHeight = 32;
    const float indent = 32;

    // Header
    {
        TextSpan textSpan = { "Connected Players:", TextRenderable::defaultColor };
        textRenderables.emplace_back(textSpan, nameProperties, renderPos.x, renderPos.y);
        renderPos.x += indent;
        renderPos.y += rowHeight;
    }

    // Local player (if hosting, should always come first)
    if (lobby.isHost())
    {
        std::string name = lobby.getLocalPlayerName();
        TextSpan textSpan = { name, TextRenderable::defaultColor };
        textRenderables.emplace_back(textSpan, nameProperties, renderPos.x, renderPos.y);
        renderPos.y += rowHeight;
    }

    // Other players
    for (const auto& entry : lobby.getClients())
    {
        std::string name = entry.second.getName();
        TextSpan textSpan = { name, TextRenderable::defaultColor };
        textRenderables.emplace_back(textSpan, nameProperties, renderPos.x, renderPos.y);
        renderPos.y += rowHeight;
    }

    // Local player (if not hosting, always comes last for now; later we will sort by player ID)
    if (!lobby.isHost())
    {
        std::string name = lobby.getLocalPlayerName();
        TextSpan textSpan = { name, TextRenderable::defaultColor };
        textRenderables.emplace_back(textSpan, nameProperties, renderPos.x, renderPos.y);
        renderPos.y += rowHeight;
    }

    // Render the text!
    std::vector<const TextRenderable*> textRenderablePtrs;
    for (const auto& textRenderable : textRenderables)
    {
        textRenderablePtrs.push_back(&textRenderable);
    }
    textRenderer.render(textRenderablePtrs);
}

}  // namespace Rival
