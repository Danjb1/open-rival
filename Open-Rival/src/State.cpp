#include "pch.h"
#include "State.h"

#include "Application.h"

namespace Rival {

    State::State(Application& app) : app(app), window(app.getWindow()), res(app.getResources()) {}

}  // namespace Rival
