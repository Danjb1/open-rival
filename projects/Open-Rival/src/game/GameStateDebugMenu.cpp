/*
 * This file includes just the ImGui code for the GameState.
 * If ImGui is disabled, it will be empty.
 */

#ifdef ENABLE_IMGUI
#include <imgui.h>

#include <string>

#include "game/GameState.h"
#include "utils/DebugUtils.h"

namespace Rival {

namespace {

static void addEntityName(std::shared_ptr<Entity> entity)
{
    if (!entity)
    {
        ImGui::TextColored(DebugUtils::red, "None");
        return;
    }

    ImGui::Text(entity->getDebugName().c_str());
}

}  // anonymous namespace

void GameState::buildImGui()
{
    if (ImGui::BeginTabBar("#tabs"))
    {
        if (ImGui::BeginTabItem("Info"))
        {
            // Tile under mouse
            if (ImGui::CollapsingHeader("Tile Under Mouse", ImGuiTreeNodeFlags_DefaultOpen))
            {
                const auto& tileUnderMouse = playerContext.tileUnderMouse;
                ImGui::Text(tileUnderMouse.toString().c_str());
            }

            // Entity under mouse
            if (ImGui::CollapsingHeader("Entity Under Mouse", ImGuiTreeNodeFlags_DefaultOpen))
            {
                addEntityName(playerContext.weakEntityUnderMouse.lock());
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

}  // namespace Rival

#endif
