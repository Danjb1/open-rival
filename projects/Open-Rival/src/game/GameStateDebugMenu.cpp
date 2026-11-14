/*
 * This file includes just the ImGui code for the GameState.
 * If ImGui is disabled, it will be empty.
 */

#ifdef ENABLE_IMGUI
#include <imgui.h>

#include <string>

#include "game/GameState.h"

namespace Rival {

void GameState::buildImGui()
{
    if (ImGui::BeginTabBar("#tabs"))
    {
        if (ImGui::BeginTabItem("Info"))
        {
            // Tile under mouse
            const auto& tileUnderMouse = playerContext.tileUnderMouse;
            ImGui::Text("Mouse is over tile: %s", tileUnderMouse.toString().c_str());

            // Entity under mouse
            std::string hoveredEntityId = "None";
            if (auto entityUnderMouse = playerContext.weakEntityUnderMouse.lock())
            {
                hoveredEntityId = std::to_string(entityUnderMouse->getId());
            }
            ImGui::Text("Mouse is over entity: %s", hoveredEntityId.c_str());

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

}  // namespace Rival

#endif
