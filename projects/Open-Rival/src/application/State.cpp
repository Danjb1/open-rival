#include "application/State.h"

#include "application/Application.h"
#include "application/ApplicationContext.h"

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

}  // namespace Rival
