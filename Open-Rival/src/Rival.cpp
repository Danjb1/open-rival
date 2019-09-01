#include "pch.h"
#include "Rival.h"

#include <map>

#include "Image.h"
#include "Palette.h"
#include "Shaders.h"

namespace Rival {

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
        initTileSprites();

        // Initialize Palette Texture
        paletteTexture = std::make_unique<Texture>(
                createPaletteTexture());

        // Create the Scene
        scenario = std::make_unique<Scenario>(64, 40, false);

        // Add Units
        scenario->addUnit(std::make_unique<Unit>(Unit::Type::LightCavalry, 0, 0));
        scenario->addUnit(std::make_unique<Unit>(Unit::Type::Knight, 1, 1));
        scenario->addUnit(std::make_unique<Unit>(Unit::Type::Peasant, 4, 2));

        // Create the UnitRenderer
        unitRenderer = std::make_unique<UnitRenderer>(
                *unitSprites.get(),
                *paletteTexture.get());

        // Create the TileRenderer
        tileRenderer = std::make_unique<TileRenderer>(
            *tileSprites.get(),
            *paletteTexture.get());
    }

    void Rival::initSDL() const {

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
    }

    void Rival::initUnitSprites() {

        int nextIndex = txIndexUnits;

        // Human
        initUnitSprite(Unit::Type::Ballista, nextIndex);
        initUnitSprite(Unit::Type::Battleship, nextIndex++);
        initUnitSprite(Unit::Type::Bowman, nextIndex++);
        initUnitSprite(Unit::Type::ChariotOfWar, nextIndex++);
        initUnitSprite(Unit::Type::FireMaster, nextIndex++);
        initUnitSprite(Unit::Type::Knight, nextIndex++);
        initUnitSprite(Unit::Type::LightCavalry, nextIndex++);
        initUnitSprite(Unit::Type::Peasant, nextIndex++);
        initUnitSprite(Unit::Type::PegasRider, nextIndex++);
        initUnitSprite(Unit::Type::Priest, nextIndex++);
        initUnitSprite(Unit::Type::SeaBarge, nextIndex++);
        initUnitSprite(Unit::Type::Thief, nextIndex++);
        initUnitSprite(Unit::Type::Wizard, nextIndex++);
        initUnitSprite(Unit::Type::Zeppelin, nextIndex++);

        // Greenskin
        initUnitSprite(Unit::Type::Balloon, nextIndex++);
        initUnitSprite(Unit::Type::Catapult, nextIndex++);
        initUnitSprite(Unit::Type::GnomeBoomer, nextIndex++);
        initUnitSprite(Unit::Type::HordeRider, nextIndex++);
        initUnitSprite(Unit::Type::LandingCraft, nextIndex++);
        initUnitSprite(Unit::Type::Necromancer, nextIndex++);
        initUnitSprite(Unit::Type::PriestOfDoom, nextIndex++);
        initUnitSprite(Unit::Type::RockThrower, nextIndex++);
        initUnitSprite(Unit::Type::Rogue, nextIndex++);
        initUnitSprite(Unit::Type::Serf, nextIndex++);
        initUnitSprite(Unit::Type::StormTrooper, nextIndex++);
        initUnitSprite(Unit::Type::TrollGalley, nextIndex++);
        initUnitSprite(Unit::Type::Warbat, nextIndex++);
        initUnitSprite(Unit::Type::Warlord, nextIndex++);

        // Elf
        initUnitSprite(Unit::Type::Archer, nextIndex++);
        initUnitSprite(Unit::Type::Arquebusier, nextIndex++);
        initUnitSprite(Unit::Type::Bark, nextIndex++);
        initUnitSprite(Unit::Type::Bombard, nextIndex++);
        initUnitSprite(Unit::Type::Centaur, nextIndex++);
        initUnitSprite(Unit::Type::Druid, nextIndex++);
        initUnitSprite(Unit::Type::DwarfMiner, nextIndex++);
        initUnitSprite(Unit::Type::Enchanter, nextIndex++);
        initUnitSprite(Unit::Type::Mage, nextIndex++);
        initUnitSprite(Unit::Type::MagicChopper, nextIndex++);
        initUnitSprite(Unit::Type::Scout, nextIndex++);
        initUnitSprite(Unit::Type::SkyRider, nextIndex++);
        initUnitSprite(Unit::Type::Warship, nextIndex++);
        initUnitSprite(Unit::Type::Yeoman, nextIndex++);

        // Monsters
        initUnitSprite(Unit::Type::Devil, nextIndex++);
        initUnitSprite(Unit::Type::Dragon, nextIndex++);
        initUnitSprite(Unit::Type::Golem, nextIndex++);
        initUnitSprite(Unit::Type::Gryphon, nextIndex++);
        initUnitSprite(Unit::Type::Hydra, nextIndex++);
        initUnitSprite(Unit::Type::SeaMonster, nextIndex++);
        initUnitSprite(Unit::Type::Skeleton, nextIndex++);
        initUnitSprite(Unit::Type::Snake, nextIndex++);
    }

    void Rival::initUnitSprite(Unit::Type type, int txIndex) {
        unitSprites->emplace(std::piecewise_construct,
            std::forward_as_tuple(type),
            std::forward_as_tuple(
                    textures->at(txIndex),
                    Sprite::unitWidthPx,
                    Sprite::unitHeightPx));
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
                Sprite::tileSpriteWidthPx,
                Sprite::tileSpriteHeightPx));
    }

    void Rival::start() {

        // Event handler
        SDL_Event e;

        // Run our game loop until the application is exited
        bool exiting = false;
        while (!exiting) {

            // Handle events on queue
            while (SDL_PollEvent(&e) != 0) {

                // User requests exiting
                if (e.type == SDL_QUIT) {
                    exiting = true;
                } else if (e.type == SDL_KEYDOWN) {
                    keyDown(e.key.keysym.sym);
                }
            }

            update();
            render();
            window->swapBuffers();
        }

        // Free resources and exit SDL
        exit();
    }

    void Rival::update() {
        std::map<int, std::unique_ptr<Unit>>& units = scenario->getUnits();
        for (auto const& kv : units) {
            const std::unique_ptr<Unit>& unit = kv.second;
            //unit->rotateRight();
        }
    }

    void Rival::render() {
        glClear(GL_COLOR_BUFFER_BIT);
        tileRenderer->render(
                scenario->getTiles(),
                scenario->getWidth(),
                scenario->getHeight(),
                scenario->isWilderness());
        unitRenderer->render(scenario->getUnits());
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
            std::cout << "UP\n";
            break;

        case SDLK_DOWN:
            std::cout << "DOWN\n";
            break;

        case SDLK_LEFT:
            std::cout << "LEFT\n";
            break;

        case SDLK_RIGHT:
            std::cout << "RIGHT\n";
            break;

        default:
            break;
        }
    }

}
