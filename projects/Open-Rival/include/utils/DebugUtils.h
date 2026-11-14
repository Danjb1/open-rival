#pragma once

#ifdef ENABLE_IMGUI
#include <imgui.h>
#endif

namespace Rival { namespace DebugUtils {

#ifdef ENABLE_IMGUI
static constexpr ImVec4 green { 0.f, 1.f, 0.f, 1.f };
static constexpr ImVec4 red { 1.f, 0.f, 0.f, 1.f };
#endif

}}  // namespace Rival::DebugUtils
