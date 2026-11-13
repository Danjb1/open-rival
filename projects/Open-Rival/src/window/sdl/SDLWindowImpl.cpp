#include "window/sdl/SDLWindowImpl.h"

#include "utils/SDLWrapper.h"
#include <SDL3_image/SDL_image.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>

#include "application/ResourceLoader.h"
#include "utils/LogUtils.h"

namespace Rival {

namespace {

KeyCode toKeyCode(SDL_Keycode sdlKey)
{
    if (sdlKey >= SDLK_A && sdlKey <= SDLK_Z)
    {
        return static_cast<KeyCode>(static_cast<int>(KeyCode::A) + static_cast<int>(sdlKey - SDLK_A));
    }

    if (sdlKey >= SDLK_0 && sdlKey <= SDLK_9)
    {
        return static_cast<KeyCode>(static_cast<int>(KeyCode::Num0) + static_cast<int>(sdlKey - SDLK_0));
    }

    if (sdlKey >= SDLK_F1 && sdlKey <= SDLK_F12)
    {
        return static_cast<KeyCode>(static_cast<int>(KeyCode::F1) + static_cast<int>(sdlKey - SDLK_F1));
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
    case SDLK_GRAVE:
        return KeyCode::Backtick;
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
    // SDL3 reports mouse positions as floats, but we only care about whole pixels
    return { toMouseButton(sdlEvent.button), static_cast<int>(sdlEvent.x), static_cast<int>(sdlEvent.y) };
}

MouseMoveEvent toMouseMoveEvent(const SDL_MouseMotionEvent& sdlEvent)
{
    return { static_cast<int>(sdlEvent.x), static_cast<int>(sdlEvent.y) };
}

MouseWheelEvent toMouseWheelEvent(const SDL_MouseWheelEvent& sdlEvent)
{
    // SDL3 reports precise (fractional) scroll amounts in `y`, e.g. for touchpads.
    // `integer_y` accumulates these into whole scroll "ticks", matching SDL2's behaviour.
    int delta = sdlEvent.integer_y;
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

    sdlWindow = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);

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
    if (SDL_GL_SetSwapInterval(1))
    {
        vsyncEnabled = true;
    }
    else
    {
        LOG_WARN("Failed to enable vsync! SDL Error: {}", SDL_GetError());
    }

    // Initialize the ImGui platform backend
    if (!ImGui_ImplSDL3_InitForOpenGL(sdlWindow, glContext))
    {
        throw std::runtime_error("Failed to initialize ImGui SDL3 backend");
    }
}

SDLWindowImpl::~SDLWindowImpl()
{
    ImGui_ImplSDL3_Shutdown();

    SDL_GL_DestroyContext(glContext);
    glContext = nullptr;

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

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        LOG_ERROR("Failed to initialize SDL");
        throw std::runtime_error(SDL_GetError());
    }

    // Use OpenGL 3.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Hide cursor
    SDL_HideCursor();

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
    while (SDL_PollEvent(&e))
    {
        // ImGui needs to see every event so it can track the state of the mouse, keyboard, etc.
        ImGui_ImplSDL3_ProcessEvent(&e);

        // If there is no listener, we just drain the queue
        if (listener)
        {
            handleEvent(e);
        }
    }
}

void SDLWindowImpl::handleEvent(const SDL_Event& e)
{
    // ImGui tells us when it wants exclusive use of the mouse / keyboard (e.g. when hovering over an ImGui window, or
    // typing into an ImGui text field), in which case we should not pass those inputs on to the game.
    const ImGuiIO& io = ImGui::GetIO();

    switch (e.type)
    {
    case SDL_EVENT_QUIT:
        listener->onExitRequested();
        break;

    case SDL_EVENT_WINDOW_FOCUS_LOST:
        keysForwardedToListener.clear();
        listener->onFocusLost();
        break;

    case SDL_EVENT_KEY_DOWN:
        if (!io.WantCaptureKeyboard)
        {
            keysForwardedToListener.insert(e.key.key);
            listener->onKeyDownEvent(KeyEvent(toKeyCode(e.key.key)));
        }
        break;

    case SDL_EVENT_KEY_UP:
        // Only pass on key-ups if we passed on the key-down. This way, a key pressed in-game and released while ImGui
        // has focus doesn't get stuck, and a key pressed within ImGui doesn't trigger a key-up action in-game.
        if (keysForwardedToListener.erase(e.key.key) > 0)
        {
            listener->onKeyUpEvent(KeyEvent(toKeyCode(e.key.key)));
        }
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (!io.WantCaptureMouse)
        {
            listener->onMouseDownEvent(toMouseButtonEvent(e.button));
        }
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
        // ImGui only claims a button release if it also claimed the press, so this keeps button events paired
        if (!io.WantCaptureMouse)
        {
            listener->onMouseUpEvent(toMouseButtonEvent(e.button));
        }
        break;

    case SDL_EVENT_MOUSE_MOTION:
        // Always pass this on, since the game renders its own cursor (even on top of ImGui)
        listener->onMouseMoveEvent(toMouseMoveEvent(e.motion));
        break;

    case SDL_EVENT_MOUSE_WHEEL:
        if (!io.WantCaptureMouse)
        {
            listener->onMouseWheelEvent(toMouseWheelEvent(e.wheel));
        }
        break;

    default:
        break;
    }
}

void SDLWindowImpl::beginImGuiFrame()
{
    ImGui_ImplSDL3_NewFrame();
}

}  // namespace Rival
