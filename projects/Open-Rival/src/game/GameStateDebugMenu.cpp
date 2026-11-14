/*
 * This file includes just the ImGui code for the GameState.
 * If ImGui is disabled, it will be empty.
 */

#ifdef ENABLE_IMGUI
#include <imgui.h>
#include <magic_enum/magic_enum.hpp>

#include <string>

#include "game/GameState.h"
#include "utils/DebugUtils.h"

namespace Rival {

namespace {

static void addPassability(TilePassability passability)
{
    for (const TilePassability flag : magic_enum::enum_values<TilePassability>())
    {
        bool isFlagPresent = (passability & flag) != TilePassability::Clear;
        auto color = isFlagPresent ? DebugUtils::green : DebugUtils::red;
        ImGui::TextColored(color, "%s", magic_enum::enum_name(flag).data());
    }
}

static void addEntityName(const Entity& entity)
{
    ImGui::Text(entity.getDebugName().c_str());
}

static void addEntityInfo(const Entity& entity)
{
    if (const Unit* unit = entity.as<Unit>())
    {
        ImGui::Text("State: %s", magic_enum::enum_name(unit->getState()).data());
    }
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
                auto passability = world->getPassabilityMap()->getPassability(tileUnderMouse);
                addPassability(passability);
            }

            // Entity under mouse
            if (ImGui::CollapsingHeader("Entity Under Mouse", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (auto hoveredEntity = playerContext.weakEntityUnderMouse.lock())
                {
                    addEntityName(*hoveredEntity);
                    addEntityInfo(*hoveredEntity);
                }
                else
                {
                    ImGui::Text("None");
                }
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

}  // namespace Rival

#endif
