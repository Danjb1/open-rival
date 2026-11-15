#pragma once

#include <memory>
#include <string>

#include "application/State.h"

namespace Rival {

struct ScenarioData;

/**
 * State active while in the main menu screen.
 */
class MainMenuState : public State
{
public:
    MainMenuState(Application& app);

    // Begin State override
    void onLoad() override;
    void onKeyEvent(const KeyEvent& evt) override;
    void update(int delta) override;
    void render(const FrameTiming& frameTiming) override;
    // End State override

private:
    std::unique_ptr<UIRoot> createUI() const;
    void startGame();
    std::unique_ptr<State> createGameState() const;
    ScenarioData loadLevel(const std::string& filename) const;

private:
    std::unique_ptr<UIRoot> uiRoot;
};

}  // namespace Rival
