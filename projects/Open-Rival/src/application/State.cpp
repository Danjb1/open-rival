#include "application/State.h"

#include "application/Application.h"
#include "gfx/RenderUtils.h"

namespace Rival {

State::State(Application& app)
    : app(app)
    , window(app.getWindow())
    , res(app.getResources())
{
}

const Resources& State::getResources() const
{
    return res;
}

glm::ivec2 State::windowToUI(const glm::ivec2& position) const
{
    const glm::ivec2 uiSize = RenderUtils::getUISize(window->getAspectRatio());
    const glm::ivec2 windowSize { window->getWidth(), window->getHeight() };
    return position * uiSize / windowSize;
}

}  // namespace Rival
