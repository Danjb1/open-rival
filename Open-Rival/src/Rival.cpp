#include "pch.h"
#include "Rival.h"

#include <gl/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <stdexcept>

#include "Image.h"
#include "Palette.h"
#include "Race.h"
#include "RenderUtils.h"
#include "ScenarioBuilder.h"
#include "ScenarioData.h"
#include "ScenarioReader.h"
#include "Shaders.h"

namespace Rival {

    static const double aspectRatio =
            static_cast<double>(Rival::windowWidth) / Rival::windowHeight;

    void Rival::initialize() {

        // Initialize SDL
        initSDL();

        // Create the Window
        window = std::make_unique<Window>(
                windowWidth, windowHeight, windowTitle);
        window->use();

        // Initialize OpenGL
        initGLEW();
        initGL();

        // Load resources
        loadTextures();
        initUnitSprites();
        initMapBorderSpritesheet();
        initTileSprites();

        // Initialize Palette Texture
        paletteTexture = std::make_unique<Texture>(
                createPaletteTexture());

        // Load some scenario
        ScenarioReader reader(mapsDir + "example.sco");
        ScenarioData scenarioData = reader.readScenario();
        ScenarioBuilder scenarioBuilder(scenarioData);
        scenario = scenarioBuilder.build();

        // Create our framebuffer
        gameFbo = std::make_unique<Framebuffer>(
                Framebuffer::generate(framebufferWidth, framebufferHeight));

        // Create the FramebufferRenderer
        gameFboRenderer = std::make_unique<FramebufferRenderer>(
                *gameFbo.get());
        gameFboRenderer->init();

        // Create the Camera
        const float cameraWidth = RenderUtils::pxToWorld_X(windowWidth);
        camera = std::make_unique<Camera>(
                0.0f, 0.0f, cameraWidth, aspectRatio, *scenario.get());

        // Create the MousePicker
        mousePicker = std::make_unique<MousePicker>(
                *camera.get(),
                scenario->getWidth(),
                scenario->getHeight());

        // Pick which tile Spritesheet to use based on the map type
        const Spritesheet& tileSpritesheet = scenario->isWilderness()
                ? tileSprites->at(1)
                : tileSprites->at(0);

        // Create the TileRenderer
        tileRenderer = std::make_unique<TileRenderer>(
                tileSpritesheet,
                *paletteTexture.get());

        // Hardcode this, for now
        Race race = Race::Human;

        // Create the MapBorderRenderer
        mapBorderRenderer = std::make_unique<MapBorderRenderer>(
                race,
                scenario->getWidth(),
                scenario->getHeight(),
                *mapBorderSpritesheet.get(),
                *paletteTexture.get());

        // Create the UnitRenderer
        unitRenderer = std::make_unique<UnitRenderer>(
                *unitSprites.get(),
                *paletteTexture.get());
    }

    void Rival::initSDL() const {

        // This must be called before SDL_Init since we're not using SDL_main
        // as an entry point
        SDL_SetMainReady();

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialise!\n";
            throw std::runtime_error(SDL_GetError());
        }

        // Use OpenGL 3.1 core
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE);
    }

    void Rival::initGLEW() const {

        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();

        if (glewError != GLEW_OK) {
            std::cerr << "Error initialising GLEW:"
                    << glewGetErrorString(glewError) << "\n";
            throw std::runtime_error("Failed to initialise GLEW");
        }
    }

    void Rival::initGL() {

        // Enable vsync
        if (SDL_GL_SetSwapInterval(1) < 0) {
            printf("Unable to enable vsync! SDL Error: %s\n", SDL_GetError());
        }

        // Set clear color
        glClearColor(0.5f, 0.5f, 1.f, 1.f);

        // Enable back-face culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);

        // Enable texturing
        glEnable(GL_TEXTURE_2D);

        // Enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        initialiseShaders();
    }

    void Rival::loadTextures() {

        textures->reserve(numTextures);

        // Units - Human
        textures->push_back(Texture::loadTexture(txDir + "unit_human_ballista.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_battleship.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_bowman.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_chariot_of_war.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_fire_master.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_knight.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_light_cavalry.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_peasant.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_pegas_rider.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_priest.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_sea_barge.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_thief.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_wizard.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_human_zeppelin.tga"));

        // Units - Greenskin
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_balloon.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_catapult.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_gnome_boomer.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_horde_rider.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_landing_craft.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_necromancer.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_priest_of_doom.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_rock_thrower.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_rogue.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_serf.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_storm_trooper.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_troll_galley.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_warbat.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_greenskin_warlord.tga"));

        // Units - Elf
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_archer.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_arquebusier.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_bark.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_bombard.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_centaur.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_druid.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_dwarf_miner.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_enchanter.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_mage.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_magic_chopper.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_scout.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_sky_rider.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_warship.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_elf_yeoman.tga"));

        // Units - Monsters
        textures->push_back(Texture::loadTexture(txDir + "unit_monster_devil.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_monster_dragon.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_monster_golem.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_monster_gryphon.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_monster_hydra.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_monster_sea_monster.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_monster_skeleton.tga"));
        textures->push_back(Texture::loadTexture(txDir + "unit_monster_snake.tga"));

        // Tiles
        textures->push_back(Texture::loadTexture(txDir + "tiles_meadow.tga"));
        textures->push_back(Texture::loadTexture(txDir + "tiles_wilderness.tga"));
        textures->push_back(Texture::loadTexture(txDir + "tiles_fog.tga"));

        // UI
        textures->push_back(Texture::loadTexture(txDir + "ui_cursor_select.tga"));
        textures->push_back(Texture::loadTexture(txDir + "ui_map_border.tga"));
    }

    void Rival::initUnitSprites() {

        int nextIndex = txIndexUnits;

        // Human
        initUnitSprite(UnitType::Ballista, nextIndex++);
        initUnitSprite(UnitType::Battleship, nextIndex++);
        initUnitSprite(UnitType::Bowman, nextIndex++);
        initUnitSprite(UnitType::ChariotOfWar, nextIndex++);
        initUnitSprite(UnitType::FireMaster, nextIndex++);
        initUnitSprite(UnitType::Knight, nextIndex++);
        initUnitSprite(UnitType::LightCavalry, nextIndex++);
        initUnitSprite(UnitType::Peasant, nextIndex++);
        initUnitSprite(UnitType::PegasRider, nextIndex++);
        initUnitSprite(UnitType::Priest, nextIndex++);
        initUnitSprite(UnitType::SeaBarge, nextIndex++);
        initUnitSprite(UnitType::Thief, nextIndex++);
        initUnitSprite(UnitType::Wizard, nextIndex++);
        initUnitSprite(UnitType::Zeppelin, nextIndex++);

        // Greenskin
        initUnitSprite(UnitType::Balloon, nextIndex++);
        initUnitSprite(UnitType::Catapult, nextIndex++);
        initUnitSprite(UnitType::GnomeBoomer, nextIndex++);
        initUnitSprite(UnitType::HordeRider, nextIndex++);
        initUnitSprite(UnitType::LandingCraft, nextIndex++);
        initUnitSprite(UnitType::Necromancer, nextIndex++);
        initUnitSprite(UnitType::PriestOfDoom, nextIndex++);
        initUnitSprite(UnitType::RockThrower, nextIndex++);
        initUnitSprite(UnitType::Rogue, nextIndex++);
        initUnitSprite(UnitType::Serf, nextIndex++);
        initUnitSprite(UnitType::StormTrooper, nextIndex++);
        initUnitSprite(UnitType::TrollGalley, nextIndex++);
        initUnitSprite(UnitType::Warbat, nextIndex++);
        initUnitSprite(UnitType::Warlord, nextIndex++);

        // Elf
        initUnitSprite(UnitType::Archer, nextIndex++);
        initUnitSprite(UnitType::Arquebusier, nextIndex++);
        initUnitSprite(UnitType::Bark, nextIndex++);
        initUnitSprite(UnitType::Bombard, nextIndex++);
        initUnitSprite(UnitType::Centaur, nextIndex++);
        initUnitSprite(UnitType::Druid, nextIndex++);
        initUnitSprite(UnitType::DwarfMiner, nextIndex++);
        initUnitSprite(UnitType::Enchanter, nextIndex++);
        initUnitSprite(UnitType::Mage, nextIndex++);
        initUnitSprite(UnitType::MagicChopper, nextIndex++);
        initUnitSprite(UnitType::Scout, nextIndex++);
        initUnitSprite(UnitType::SkyRider, nextIndex++);
        initUnitSprite(UnitType::Warship, nextIndex++);
        initUnitSprite(UnitType::Yeoman, nextIndex++);

        // Monsters
        initUnitSprite(UnitType::Devil, nextIndex++);
        initUnitSprite(UnitType::Dragon, nextIndex++);
        initUnitSprite(UnitType::Golem, nextIndex++);
        initUnitSprite(UnitType::Gryphon, nextIndex++);
        initUnitSprite(UnitType::Hydra, nextIndex++);
        initUnitSprite(UnitType::SeaMonster, nextIndex++);
        initUnitSprite(UnitType::Skeleton, nextIndex++);
        initUnitSprite(UnitType::Snake, nextIndex++);
    }

    void Rival::initUnitSprite(UnitType type, int txIndex) {
        unitSprites->emplace(std::piecewise_construct,
            std::forward_as_tuple(type),
            std::forward_as_tuple(
                    textures->at(txIndex),
                    RenderUtils::unitWidthPx,
                    RenderUtils::unitHeightPx));
    }

    void Rival::initMapBorderSpritesheet() {
        mapBorderSpritesheet = std::make_unique<Spritesheet>(
                textures->at(txIndexUi + 1),
                RenderUtils::tileSpriteWidthPx,
                RenderUtils::tileSpriteHeightPx);
    }

    void Rival::initTileSprites() {
        int nextIndex = txIndexTiles;
        initTileSprite(0, nextIndex++); // Meadow
        initTileSprite(1, nextIndex++); // Wilderness
        initTileSprite(2, nextIndex++); // Fog
    }

    void Rival::initTileSprite(int type, int txIndex) {
        tileSprites->emplace(std::piecewise_construct,
            std::forward_as_tuple(type),
            std::forward_as_tuple(
                    textures->at(txIndex),
                    RenderUtils::tileSpriteWidthPx,
                    RenderUtils::tileSpriteHeightPx));
    }

    void Rival::start() {

        // Event handler
        SDL_Event e;

        // Run our game loop until the application is exited
        bool exiting = false;
        while (!exiting) {

            // Handle events on queue
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    exiting = true;
                } else if (e.type == SDL_KEYDOWN) {
                    keyDown(e.key.keysym.sym);
                } else if (e.type == SDL_MOUSEWHEEL) {
                    mouseWheelMoved(e.wheel);
                }
            }

            // Process the current frame
            update();
            render();

            // Update the window with our newly-rendered game;
            // this will block execution until the next refresh interval,
            // due to vsync
            window->swapBuffers();
        }

        // Free resources and exit SDL
        exit();
    }

    void Rival::update() {

        mousePicker->handleMouse();
        std::cout << mousePicker->getTileX() << ", " << mousePicker->getTileY() << "\n";

        std::map<int, std::unique_ptr<Unit>>& units = scenario->getUnits();
        for (auto const& kv : units) {
            const std::unique_ptr<Unit>& unit = kv.second;
            //unit->rotateRight();
        }
    }

    void Rival::render() {

        // Render to our framebuffer.
        // Here the viewport specifies the region of the framebuffer texture
        // that we render onto, in pixels. We use the camera size here; if the
        // camera is wider, more pixels are visible, and thus we need a larger
        // render target.
        glBindFramebuffer(GL_FRAMEBUFFER, gameFbo->getId());
        int viewportWidth = static_cast<int>(
                RenderUtils::worldToPx_X(camera->getWidth()));
        int viewportHeight = static_cast<int>(
                RenderUtils::worldToPx_Y(camera->getHeight()));
        glViewport(0, 0, viewportWidth, viewportHeight);
        renderGame(viewportWidth, viewportHeight);

        // Render the framebuffer to the screen.
        // Here the viewport specifies the region of the game window that we
        // render onto, in pixels.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, windowWidth, windowHeight);
        renderFramebuffer();
    }

    void Rival::renderGame(int viewportWidth, int viewportHeight) {

        // Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Use indexed texture shader
        glUseProgram(indexedTextureShader.programId);

        // Determine view matrix.
        // OpenGL uses right-handed rule:
        //  - x points right
        //  - y points up
        //  - z points out of the screen
        // The camera co-ordinates are in world units (tiles), but our vertices
        // are positioned using pixels. Therefore we need to convert the camera
        // co-ordinates to pixels, too.
        float cameraX = RenderUtils::worldToPx_X(camera->getX());
        float cameraY = RenderUtils::worldToPx_Y(camera->getY());
        glm::mat4 view = glm::lookAt(
            glm::vec3(cameraX, cameraY, 1),     // camera position
            glm::vec3(cameraX, cameraY, 0),     // look at
            glm::vec3(0, 1, 0)                  // up vector
        );

        // Determine projection matrix.
        // This should match the viewport size exactly, in order to achieve
        // pixel-perfect rendering.
        float left = -viewportWidth / 2.0f;
        float top = -viewportHeight / 2.0f;
        float right = viewportWidth / 2.0f;
        float bottom = viewportHeight / 2.0f;
        glm::mat4 projection = glm::ortho(left, right, bottom, top);

        // Combine matrices
        glm::mat4 viewProjMatrix = projection * view;

        // Set uniform values
        glUniformMatrix4fv(indexedTextureShader.viewProjMatrixUniformLocation,
                1, GL_FALSE, &viewProjMatrix[0][0]);
        glUniform1i(indexedTextureShader.texUnitUniformLocation, 0);
        glUniform1i(indexedTextureShader.paletteTexUnitUniformLocation, 1);

        // Render Tiles
        tileRenderer->render(
                *camera.get(),
                scenario->getTiles(),
                scenario->getWidth(),
                scenario->getHeight());

        // Render Map Borders
        mapBorderRenderer->render();

        // Render Units
        unitRenderer->render(*camera.get(), scenario->getUnits());
    }

    void Rival::renderFramebuffer() {

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Use screen shader
        glUseProgram(screenShader.programId);

        // Set uniform values
        glUniform1i(screenShader.texUnitUniformLocation, 0);

        // Render framebuffer to screen.
        // At a zoom level of 1, this will result in pixel-perfect rendering.
        // A higher zoom level will result in a smaller sample, which will
        // then be stretched to fill the viewport.
        float srcWidth = windowWidth / camera->getZoom();
        float srcHeight = windowHeight / camera->getZoom();
        gameFboRenderer->render(srcWidth, srcHeight);
    }

    void Rival::exit() {

        // Destroy window
        window.reset();

        // Delete Textures
        for (Texture& texture : *textures.get()) {
            const GLuint texId = texture.getId();
            glDeleteTextures(1, &texId);
        }
        textures->clear();

        // Quit SDL
        SDL_Quit();
    }

    void Rival::keyDown(const SDL_Keycode keyCode) const {
        switch (keyCode) {
        case SDLK_UP:
            camera->translate(0.0f, -0.5f);
            break;

        case SDLK_DOWN:
            camera->translate(0.0f, 0.5f);
            break;

        case SDLK_LEFT:
            camera->translate(-0.5f, 0.0f);
            break;

        case SDLK_RIGHT:
            camera->translate(0.5f, 0.0f);
            break;

        default:
            break;
        }
    }

    void Rival::mouseWheelMoved(const SDL_MouseWheelEvent evt) const {

        // Determine the normalized scroll amount
        int dy = evt.y;
        if (evt.direction == SDL_MOUSEWHEEL_FLIPPED) {
            dy *= -1;
        }

        if (dy > 0) {
            camera->modZoom(Camera::zoomInterval);
        } else if (dy < 0) {
            camera->modZoom(-Camera::zoomInterval);
        }
    }

}
