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

        // Allocate space for Textures
        textures->reserve(numTextures);

        // Initialize Sprites
        initSprites();

        // Initialize Palette Texture
        paletteTexture = std::make_unique<Texture>(
                createPaletteTexture());

        // Create the Scene
        scene = std::make_unique<Scene>();

        // Add Units
        scene->addUnit(std::make_unique<Unit>(Unit::Type::LightCavalry, 0, 0));
        scene->addUnit(std::make_unique<Unit>(Unit::Type::Knight, 1, 1));

        // Create the UnitRenderer
        unitRenderer = std::make_unique<UnitRenderer>(
                *unitSprites.get(),
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
        //glEnable(GL_CULL_FACE);

        // Enable texturing
        glEnable(GL_TEXTURE_2D);

        // Enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        initialiseShaders();
    }

    void Rival::initSprites() {

        // Human
        loadUnit(Unit::Type::Ballista, "unit_human_ballista.tga");
        loadUnit(Unit::Type::Battleship, "unit_human_battleship.tga");
        loadUnit(Unit::Type::Bowman, "unit_human_bowman.tga");
        loadUnit(Unit::Type::ChariotOfWar, "unit_human_chariot_of_war.tga");
        loadUnit(Unit::Type::FireMaster, "unit_human_fire_master.tga");
        loadUnit(Unit::Type::Knight, "unit_human_knight.tga");
        loadUnit(Unit::Type::LightCavalry, "unit_human_light_cavalry.tga");
        loadUnit(Unit::Type::Peasant, "unit_human_peasant.tga");
        loadUnit(Unit::Type::PegasRider, "unit_human_pegas_rider.tga");
        loadUnit(Unit::Type::Priest, "unit_human_priest.tga");
        loadUnit(Unit::Type::SeaBarge, "unit_human_sea_barge.tga");
        loadUnit(Unit::Type::Thief, "unit_human_thief.tga");
        loadUnit(Unit::Type::Wizard, "unit_human_wizard.tga");
        loadUnit(Unit::Type::Zeppelin, "unit_human_zeppelin.tga");

        // Greenskin
        loadUnit(Unit::Type::Balloon, "unit_greenskin_balloon.tga");
        loadUnit(Unit::Type::Catapult, "unit_greenskin_catapult.tga");
        loadUnit(Unit::Type::GnomeBoomer, "unit_greenskin_gnome_boomer.tga");
        loadUnit(Unit::Type::HordeRider, "unit_greenskin_horde_rider.tga");
        loadUnit(Unit::Type::LandingCraft, "unit_greenskin_landing_craft.tga");
        loadUnit(Unit::Type::Necromancer, "unit_greenskin_necromancer.tga");
        loadUnit(Unit::Type::PriestOfDoom, "unit_greenskin_priest_of_doom.tga");
        loadUnit(Unit::Type::RockThrower, "unit_greenskin_rock_thrower.tga");
        loadUnit(Unit::Type::Rogue, "unit_greenskin_rogue.tga");
        loadUnit(Unit::Type::Serf, "unit_greenskin_serf.tga");
        loadUnit(Unit::Type::StormTrooper, "unit_greenskin_storm_trooper.tga");
        loadUnit(Unit::Type::TrollGalley, "unit_greenskin_troll_galley.tga");
        loadUnit(Unit::Type::Warbat, "unit_greenskin_warbat.tga");
        loadUnit(Unit::Type::Warlord, "unit_greenskin_warlord.tga");

        // Elf
        loadUnit(Unit::Type::Archer, "unit_elf_archer.tga");
        loadUnit(Unit::Type::Arquebusier, "unit_elf_arquebusier.tga");
        loadUnit(Unit::Type::Bark, "unit_elf_bark.tga");
        loadUnit(Unit::Type::Bombard, "unit_elf_bombard.tga");
        loadUnit(Unit::Type::Centaur, "unit_elf_centaur.tga");
        loadUnit(Unit::Type::Druid, "unit_elf_druid.tga");
        loadUnit(Unit::Type::DwarfMiner, "unit_elf_dwarf_miner.tga");
        loadUnit(Unit::Type::Enchanter, "unit_elf_enchanter.tga");
        loadUnit(Unit::Type::Mage, "unit_elf_mage.tga");
        loadUnit(Unit::Type::MagicChopper, "unit_elf_magic_chopper.tga");
        loadUnit(Unit::Type::Scout, "unit_elf_scout.tga");
        loadUnit(Unit::Type::SkyRider, "unit_elf_sky_rider.tga");
        loadUnit(Unit::Type::Warship, "unit_elf_warship.tga");
        loadUnit(Unit::Type::Yeoman, "unit_elf_yeoman.tga");

        // Monsters
        loadUnit(Unit::Type::Devil, "unit_monster_devil.tga");
        loadUnit(Unit::Type::Dragon, "unit_monster_dragon.tga");
        loadUnit(Unit::Type::Golem, "unit_monster_golem.tga");
        loadUnit(Unit::Type::Gryphon, "unit_monster_gryphon.tga");
        loadUnit(Unit::Type::Hydra, "unit_monster_hydra.tga");
        loadUnit(Unit::Type::SeaMonster, "unit_monster_sea_monster.tga");
        loadUnit(Unit::Type::Skeleton, "unit_monster_skeleton.tga");
        loadUnit(Unit::Type::Snake, "unit_monster_snake.tga");
    }

    void Rival::loadUnit(Unit::Type type, std::string imageFilename) {
        textures->push_back(loadTexture("res\\textures\\" + imageFilename));
        int index = textures->size() - 1;
        unitSprites->emplace(std::piecewise_construct,
            std::forward_as_tuple(type),
            std::forward_as_tuple(
                    textures->at(index),
                    Sprite::unitWidthPx,
                    Sprite::unitHeightPx));
    }

    const Texture Rival::loadTexture(const std::string filename) const {

        // Load image data
        Image img = Image::loadImage(filename);

        // Generate texture
        GLuint textureId = 0;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                img.getWidth(), img.getHeight(),
                0, GL_RED, GL_UNSIGNED_BYTE, img.getData()->data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Check for error
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("Error loading texture from %p pixels! %s\n",
                    img.getData(), gluErrorString(error));
            throw std::runtime_error("Failed to load texture");
        }

        return Texture(textureId, img.getWidth(), img.getHeight());
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
        std::map<int, std::unique_ptr<Unit>>& units = scene->getUnits();
        for (auto const& kv : units) {
            const std::unique_ptr<Unit>& unit = kv.second;
            unit->rotateRight();
        }
    }

    void Rival::render() {
        glClear(GL_COLOR_BUFFER_BIT);
        unitRenderer->render(scene->getUnits());
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
