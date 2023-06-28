#include "State.h"

#include "Application.h"
#include "ApplicationContext.h"

namespace Rival {

State::State(Application& app)
    : app(app)
    , window(app.getContext().getWindow())
    , res(app.getContext().getResources())
{
}

}  // namespace Rival
