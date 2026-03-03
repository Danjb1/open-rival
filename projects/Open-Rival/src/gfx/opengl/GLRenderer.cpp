#include "gfx/opengl/GLRenderer.h"

#include "game/GameState.h"
#include "gfx/opengl//GLUtils.h"
#include "gfx/opengl/Shaders.h"
#include "utils/LogUtils.h"

namespace Rival {

/*

Texture ::~Texture()
{
if (id == 0)
{
    return;
}

glDeleteTextures(1, &id);
}

const GLuint Texture::getId() const
{
return id;
}

*/

GLRenderer::GLRenderer(Window* window)
    : window(window)
{
    initGLEW();
    initGL();
}

void GLRenderer::renderLobby(int)
{
    // TODO: we need to get a reference to the LobbyState and create a MenuRenderer!
    // lobbyMenuRenderer->render(delta);
}

/* TODO -- deleted lobby code
void LobbyState::renderText()
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
    if (isHost)
    {
        std::string name = localPlayerName;
        TextSpan textSpan = { name, TextRenderable::defaultColor };
        textRenderables.emplace_back(textSpan, nameProperties, renderPos.x, renderPos.y);
        renderPos.y += rowHeight;
    }

    // Other players
    for (const auto& entry : clients)
    {
        std::string name = entry.second.getName();
        TextSpan textSpan = { name, TextRenderable::defaultColor };
        textRenderables.emplace_back(textSpan, nameProperties, renderPos.x, renderPos.y);
        renderPos.y += rowHeight;
    }

    // Local player (if not hosting, always comes last for now; later we will sort by player ID)
    if (!isHost)
    {
        std::string name = localPlayerName;
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
*/

void GLRenderer::initGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK)
    {
        const GLubyte* glewErrorString = glewGetErrorString(glewError);
        std::string errorString(reinterpret_cast<const char*>(glewErrorString));
        LOG_ERROR("Error initializing GLEW: {}", errorString);
        throw std::runtime_error("Failed to initialize GLEW");
    }
}

void GLRenderer::initGL()
{
    // Set clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Enable back-face culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    // Enable alpha blending
    // See also:
    // https://www.khronos.org/opengl/wiki/Transparency_Sorting#Alpha_test
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shaders::initializeShaders();
}

std::shared_ptr<const Texture> GLRenderer::loadTexture(const std::string& filename, const TextureProperties& props)
{
    return createTexture(Image::readImage(filename), props);
}

std::shared_ptr<const Texture> GLRenderer::createTexture(const Image& img, const TextureProperties& props)
{
    // Generate texture
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,
            0,  // target slot
            GLUtils::toGL(props.format),
            img.getWidth(),
            img.getHeight(),
            0,  // always zero!
            GLUtils::toGL(props.format),
            GL_UNSIGNED_BYTE,
            img.getData().data());

    // Set texture properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLUtils::toGL(props.magFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLUtils::toGL(props.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLUtils::toGL(props.wrapMode));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLUtils::toGL(props.wrapMode));

    // Revert the state back to normal
    glBindTexture(GL_TEXTURE_2D, 0);

    // Check for error
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        printf("Error loading texture: %s\n", gluErrorString(error));
        throw std::runtime_error("Failed to load texture");
    }

    return std::make_shared<const Texture>(textureId, img.getWidth(), img.getHeight());
}

void GLRenderer::onEnterGame(GameState* game)
{
    const World& world = game->getWorld();
    const Camera& camera = game->getCamera();
    const Rect& viewport = game->getViewport();
    const Resources& res = game->getResources();
    const PlayerContext& playerContext = game->getPlayerContext();

    gameRenderer = std::make_unique<GameRenderer>(*game, window, world, *game, camera, viewport, res, playerContext);
}

void GLRenderer::renderGame(int delta)
{
    if (gameRenderer)
    {
        gameRenderer->render(delta);
    }
}

}  // namespace Rival
