#include "gfx/opengl/GLRenderer.h"

#include "game/GameState.h"
#include "gfx/opengl/GLUtils.h"
#include "gfx/opengl/Shaders.h"
#include "lobby/LobbyState.h"
#include "utils/LogUtils.h"

namespace Rival {

GLRenderer::GLRenderer(Window* window)
    : window(window)
{
    initGLEW();
    initGL();
}

void GLRenderer::onEnterLobby(LobbyState* lobby)
{
    if (gameRenderer)
    {
        gameRenderer.reset();
    }

    lobbyRenderer = std::make_unique<LobbyRenderer>(*lobby, window);
}

void GLRenderer::renderLobby(int delta)
{
    if (lobbyRenderer)
    {
        lobbyRenderer->render(delta);
    }
}

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

// This is intended to be used as a custom deleter for a shared_ptr<const Texture> and should not be called manually
static void deleteTexture(const Texture* texture)
{
    const auto id = texture->getId();
    if (id > 0)
    {
        glDeleteTextures(1, &id);
    }

    delete texture;
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

    // shared_ptr ctor with a custom deleter for when the Texture is no longer referenced
    return std::shared_ptr<const Texture>(new Texture(textureId, img.getWidth(), img.getHeight()), deleteTexture);
}

void GLRenderer::onEnterGame(GameState* game)
{
    if (lobbyRenderer)
    {
        lobbyRenderer.reset();
    }

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
