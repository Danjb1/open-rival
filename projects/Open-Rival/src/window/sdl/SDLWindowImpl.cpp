#include "window/sdl/SDLWindowImpl.h"

#include "utils/SDLWrapper.h"
#include <SDL_image.h>

#include "application/ResourceLoader.h"
#include "utils/LogUtils.h"

namespace Rival {

namespace {

KeyCode toKeyCode(SDL_Keycode sdlKey)
{
    if (sdlKey >= SDLK_a && sdlKey <= SDLK_z)
    {
        return static_cast<KeyCode>(static_cast<int>(KeyCode::A) + (sdlKey - SDLK_a));
    }

    if (sdlKey >= SDLK_0 && sdlKey <= SDLK_9)
    {
        return static_cast<KeyCode>(static_cast<int>(KeyCode::Num0) + (sdlKey - SDLK_0));
    }

    switch (sdlKey)
    {
    case SDLK_UP:
        return KeyCode::Up;
    case SDLK_DOWN:
        return KeyCode::Down;
    case SDLK_LEFT:
        return KeyCode::Left;
    case SDLK_RIGHT:
        return KeyCode::Right;
    case SDLK_SPACE:
        return KeyCode::Space;
    case SDLK_RETURN:
        return KeyCode::Return;
    case SDLK_ESCAPE:
        return KeyCode::Escape;
    case SDLK_BACKSPACE:
        return KeyCode::Backspace;
    case SDLK_TAB:
        return KeyCode::Tab;
    case SDLK_DELETE:
        return KeyCode::Delete;
    default:
        return KeyCode::Unknown;
    }
}

MouseButton toMouseButton(Uint8 sdlButton)
{
    switch (sdlButton)
    {
    case SDL_BUTTON_LEFT:
        return MouseButton::Left;
    case SDL_BUTTON_MIDDLE:
        return MouseButton::Middle;
    case SDL_BUTTON_RIGHT:
        return MouseButton::Right;
    default:
        return MouseButton::Other;
    }
}

MouseButtonEvent toMouseButtonEvent(const SDL_MouseButtonEvent& sdlEvent)
{
    return { toMouseButton(sdlEvent.button), sdlEvent.x, sdlEvent.y };
}

MouseWheelEvent toMouseWheelEvent(const SDL_MouseWheelEvent& sdlEvent)
{
    int delta = sdlEvent.y;
    if (sdlEvent.direction == SDL_MOUSEWHEEL_FLIPPED)
    {
        delta *= -1;
    }
    return MouseWheelEvent(delta);
}

}  // anonymous namespace

SDLWindowImpl::SDLWindowImpl(int width, int height, const char* title)
    : Window(width, height, title)
{
    initSDL();

    sdlWindow = SDL_CreateWindow(title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (sdlWindow == nullptr)
    {
        LOG_ERROR("Failed to create window");
        throw std::runtime_error(SDL_GetError());
    }

    glContext = SDL_GL_CreateContext(sdlWindow);

    if (glContext == nullptr)
    {
        LOG_ERROR("Failed to create OpenGL context");
        throw std::runtime_error(SDL_GetError());
    }

    // Try to enable vsync.
    // Note that vsync may already be enabled by default!
    if (SDL_GL_SetSwapInterval(1) == 0)
    {
        vsyncEnabled = true;
    }
    else
    {
        LOG_WARN("Failed to enable vsync! SDL Error: {}", SDL_GetError());
    }

    // Set the icon
    std::string iconFilename = ResourceLoader::txDir + "icon.png";
    SDL_Surface* surface;
    surface = IMG_Load(iconFilename.c_str());
    setIcon(surface);
    SDL_FreeSurface(surface);
}

SDLWindowImpl::~SDLWindowImpl()
{
    SDL_DestroyWindow(sdlWindow);
    sdlWindow = nullptr;
}

void SDLWindowImpl::initSDL()
{
    // Ensure that we only initialize SDL once, in case we create multiple Windows.
    // Really, this initialization should be moved elsewhere.
    static bool isInitialized = false;
    if (isInitialized)
    {
        return;
    }

    LOG_DEBUG("Initializing...");

    // This must be called before SDL_Init since we're not using SDL_main
    // as an entry point
    SDL_SetMainReady();

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        LOG_ERROR("Failed to initialize SDL");
        throw std::runtime_error(SDL_GetError());
    }

    // Use OpenGL 3.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Hide cursor
    SDL_ShowCursor(SDL_DISABLE);

    isInitialized = true;
}

void SDLWindowImpl::swapBuffers() const
{
    SDL_GL_SwapWindow(sdlWindow);
}

void SDLWindowImpl::setPos(int x, int y)
{
    SDL_SetWindowPosition(sdlWindow, x, y);
}

void SDLWindowImpl::pollEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (!listener)
        {
            // Nobody to receive events; just drain the queue
            continue;
        }

        if (e.type == SDL_QUIT)
        {
            listener->onExitRequested();
        }
        else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
        {
            listener->onFocusLost();
        }
        else if (e.type == SDL_KEYDOWN)
        {
            listener->onKeyDownEvent(KeyEvent(toKeyCode(e.key.keysym.sym)));
        }
        else if (e.type == SDL_KEYUP)
        {
            listener->onKeyUpEvent(KeyEvent(toKeyCode(e.key.keysym.sym)));
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            listener->onMouseDownEvent(toMouseButtonEvent(e.button));
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            listener->onMouseUpEvent(toMouseButtonEvent(e.button));
        }
        else if (e.type == SDL_MOUSEMOTION)
        {
            listener->onMouseMoveEvent(MouseMoveEvent(e.motion.x, e.motion.y));
        }
        else if (e.type == SDL_MOUSEWHEEL)
        {
            listener->onMouseWheelEvent(toMouseWheelEvent(e.wheel));
        }
    }
}

void SDLWindowImpl::setIcon(SDL_Surface* surface)
{
    SDL_SetWindowIcon(sdlWindow, surface);
}

}  // namespace Rival
