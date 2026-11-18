#include "menus/mainmenu/MainMenuState.h"

#include "application/Application.h"
#include "application/ResourceLoader.h"
#include "entity/EntityFactory.h"
#include "game/GameState.h"
#include "game/PlayerState.h"
#include "game/World.h"
#include "gfx/RenderUtils.h"
#include "scenario/ScenarioBuilder.h"
#include "scenario/ScenarioReader.h"
#include "ui/UIRoot.h"
#include "ui/widgets/Button.h"
#include "ui/widgets/ImageView.h"
#include "utils/ConfigUtils.h"
#include "utils/LogUtils.h"

namespace Rival {

MainMenuState::MainMenuState(Application& app)
    : State(app)
    , uiRoot(createUI())
{
}

std::unique_ptr<UIRoot> MainMenuState::createUI()
{
    auto localUiRoot = std::make_unique<UIRoot>();
    const std::shared_ptr<const TextureAtlas> atlas = res.getMenuTextureAtlas();

    auto background = std::make_unique<ImageView>(ImageVisual { atlas, "menu_bg_light.tga" });
    background->setResizeMode(ResizeMode::Stretch, ResizeMode::Stretch);
    localUiRoot->addChild(std::move(background));

    // TMP: just add a single button for now.
    // We don't have a pressed image yet, so for now the button doesn't change when pressed.
    const ImageVisual buttonImage { atlas, "button.tga" };
    auto testButton = std::make_unique<Button>(buttonImage, buttonImage);
    testButton->setAnchor(Anchor::Centre);
    testButton->setOnClick([this]() { startGame(); });
    localUiRoot->addChild(std::move(testButton));

    return localUiRoot;
}

void MainMenuState::onLoad()
{
    app.getRenderer()->onEnterMainMenu(this);
}

void MainMenuState::onKeyEvent(const KeyEvent& evt)
{
    if (!evt.isPress)
    {
        switch (evt.key)
        {
        case KeyCode::Space:
        case KeyCode::Return:
            // TMP: For now, just start the game when pressing Space or Enter
            startGame();
            break;
        default:
            break;
        }
    }
}

void MainMenuState::onMouseButtonEvent(const MouseButtonEvent& evt)
{
    uiRoot->sendMouseButtonEvent({ evt.button, windowToUI(evt.position), evt.isPress });
}

void MainMenuState::onMouseMoveEvent(const MouseMoveEvent& evt)
{
    uiRoot->sendMouseMoveEvent({ windowToUI(evt.position) });
}

void MainMenuState::update(int /* delta */)
{
    const glm::ivec2 uiSize = RenderUtils::getUISize(window->getAspectRatio());
    uiRoot->update(uiSize);
}

void MainMenuState::render(const FrameTiming& frameTiming)
{
    app.getRenderer()->renderUi(*uiRoot, frameTiming);
}

void MainMenuState::startGame()
{
    std::unique_ptr<State> game = createGameState();
    app.setState(std::move(game));
}

// TMP: very rough implementation just to get us into the game
std::unique_ptr<State> MainMenuState::createGameState() const
{
    // Load the initial level
    const std::string levelName = ConfigUtils::get(app.getConfig(), "levelName", std::string());
    if (levelName.empty())
    {
        throw std::runtime_error("No level name found in config.json ");
    }
    ScenarioData scenarioData = loadLevel(levelName);

    // Create the random seed
    auto randomSeed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    LOG_INFO("Using random seed: {}", randomSeed);
    std::shared_ptr<std::mt19937> randomizer = std::make_shared<std::mt19937>(randomSeed);

    // Create the world
    ScenarioBuilder scenarioBuilder(scenarioData);
    std::shared_ptr<const EntityFactory> entityFactory =
            std::make_shared<EntityFactory>(res, app.getAudioSystem(), randomizer);
    std::unique_ptr<World> world = scenarioBuilder.build(entityFactory);

    // Initialize players
    int numPlayers = PlayerStore::maxPlayers;  // TODO: Read this from the Scenario
    std::unordered_map<int, PlayerState> playerStates;
    for (int playerId = 0; playerId < numPlayers; ++playerId)
    {
        const PlayerProperties& playerProps = scenarioData.playerProperties.at(playerId);
        playerStates.emplace(std::piecewise_construct,
                std::forward_as_tuple(playerId),
                std::forward_as_tuple(scenarioBuilder.getRace(playerProps.race),
                        static_cast<int>(playerProps.startingGold),
                        static_cast<int>(playerProps.startingWood),
                        static_cast<int>(playerProps.startingFood)));
    }

    // Empty client list for a single player game
    std::unordered_map<int, ClientInfo> clients;

    return std::make_unique<GameState>(app, std::move(world), playerStates, clients, 0, randomizer);
}

ScenarioData MainMenuState::loadLevel(const std::string& filename) const
{
    ScenarioReader reader(ResourceLoader::mapsDir + filename);
    return reader.readScenario();
}

}  // namespace Rival
