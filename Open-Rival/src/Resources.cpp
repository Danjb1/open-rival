#include "pch.h"
#include "Resources.h"

#include "RenderUtils.h"

namespace Rival {

    const std::string Resources::mapsDir = "res\\maps\\";
    const std::string Resources::txDir = "res\\textures\\";

    Resources::Resources() {
        loadTextures();
        initPaletteTexture();
        initUnitSpritesheets();
        initUiSpritesheets();
        initTileSpritesheets();
    }

    Resources::~Resources() {
        // Delete Textures
        for (Texture& texture : *textures.get()) {
            const GLuint texId = texture.getId();
            glDeleteTextures(1, &texId);
        }
        textures->clear();
    }

    void Resources::loadTextures() {

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

    void Resources::initPaletteTexture() {
        paletteTexture = std::make_unique<Texture>(
                Palette::createPaletteTexture());
    }

    void Resources::initUnitSpritesheets() {

        int nextIndex = txIndexUnits;

        // Human
        initUnitSpritesheet(UnitType::Ballista, nextIndex++);
        initUnitSpritesheet(UnitType::Battleship, nextIndex++);
        initUnitSpritesheet(UnitType::Bowman, nextIndex++);
        initUnitSpritesheet(UnitType::ChariotOfWar, nextIndex++);
        initUnitSpritesheet(UnitType::FireMaster, nextIndex++);
        initUnitSpritesheet(UnitType::Knight, nextIndex++);
        initUnitSpritesheet(UnitType::LightCavalry, nextIndex++);
        initUnitSpritesheet(UnitType::Peasant, nextIndex++);
        initUnitSpritesheet(UnitType::PegasRider, nextIndex++);
        initUnitSpritesheet(UnitType::Priest, nextIndex++);
        initUnitSpritesheet(UnitType::SeaBarge, nextIndex++);
        initUnitSpritesheet(UnitType::Thief, nextIndex++);
        initUnitSpritesheet(UnitType::Wizard, nextIndex++);
        initUnitSpritesheet(UnitType::Zeppelin, nextIndex++);

        // Greenskin
        initUnitSpritesheet(UnitType::Balloon, nextIndex++);
        initUnitSpritesheet(UnitType::Catapult, nextIndex++);
        initUnitSpritesheet(UnitType::GnomeBoomer, nextIndex++);
        initUnitSpritesheet(UnitType::HordeRider, nextIndex++);
        initUnitSpritesheet(UnitType::LandingCraft, nextIndex++);
        initUnitSpritesheet(UnitType::Necromancer, nextIndex++);
        initUnitSpritesheet(UnitType::PriestOfDoom, nextIndex++);
        initUnitSpritesheet(UnitType::RockThrower, nextIndex++);
        initUnitSpritesheet(UnitType::Rogue, nextIndex++);
        initUnitSpritesheet(UnitType::Serf, nextIndex++);
        initUnitSpritesheet(UnitType::StormTrooper, nextIndex++);
        initUnitSpritesheet(UnitType::TrollGalley, nextIndex++);
        initUnitSpritesheet(UnitType::Warbat, nextIndex++);
        initUnitSpritesheet(UnitType::Warlord, nextIndex++);

        // Elf
        initUnitSpritesheet(UnitType::Archer, nextIndex++);
        initUnitSpritesheet(UnitType::Arquebusier, nextIndex++);
        initUnitSpritesheet(UnitType::Bark, nextIndex++);
        initUnitSpritesheet(UnitType::Bombard, nextIndex++);
        initUnitSpritesheet(UnitType::Centaur, nextIndex++);
        initUnitSpritesheet(UnitType::Druid, nextIndex++);
        initUnitSpritesheet(UnitType::DwarfMiner, nextIndex++);
        initUnitSpritesheet(UnitType::Enchanter, nextIndex++);
        initUnitSpritesheet(UnitType::Mage, nextIndex++);
        initUnitSpritesheet(UnitType::MagicChopper, nextIndex++);
        initUnitSpritesheet(UnitType::Scout, nextIndex++);
        initUnitSpritesheet(UnitType::SkyRider, nextIndex++);
        initUnitSpritesheet(UnitType::Warship, nextIndex++);
        initUnitSpritesheet(UnitType::Yeoman, nextIndex++);

        // Monsters
        initUnitSpritesheet(UnitType::Devil, nextIndex++);
        initUnitSpritesheet(UnitType::Dragon, nextIndex++);
        initUnitSpritesheet(UnitType::Golem, nextIndex++);
        initUnitSpritesheet(UnitType::Gryphon, nextIndex++);
        initUnitSpritesheet(UnitType::Hydra, nextIndex++);
        initUnitSpritesheet(UnitType::SeaMonster, nextIndex++);
        initUnitSpritesheet(UnitType::Skeleton, nextIndex++);
        initUnitSpritesheet(UnitType::Snake, nextIndex++);
    }

    void Resources::initUnitSpritesheet(UnitType type, int txIndex) {
        unitSpritesheets->emplace(std::piecewise_construct,
            std::forward_as_tuple(type),
            std::forward_as_tuple(
                    textures->at(txIndex),
                    RenderUtils::unitWidthPx,
                    RenderUtils::unitHeightPx));
    }

    void Resources::initUiSpritesheets() {
        mapBorderSpritesheet = std::make_unique<Spritesheet>(
                textures->at(txIndexUi + 1),
                RenderUtils::tileSpriteWidthPx,
                RenderUtils::tileSpriteHeightPx);
    }

    void Resources::initTileSpritesheets() {
        int nextIndex = txIndexTiles;
        initTileSpritesheet(0, nextIndex++); // Meadow
        initTileSpritesheet(1, nextIndex++); // Wilderness
        initTileSpritesheet(2, nextIndex++); // Fog
    }

    void Resources::initTileSpritesheet(int type, int txIndex) {
        tileSpritesheets->emplace(std::piecewise_construct,
            std::forward_as_tuple(type),
            std::forward_as_tuple(
                    textures->at(txIndex),
                    RenderUtils::tileSpriteWidthPx,
                    RenderUtils::tileSpriteHeightPx));
    }

    Spritesheet& Resources::getTileSpritesheet(int index) {
        return tileSpritesheets->at(index);
    }

    std::map<UnitType, Spritesheet>& Resources::getUnitSpritesheets() {
        return *unitSpritesheets.get();
    }

    Spritesheet& Resources::getMapBorderSpritesheet() {
        return *mapBorderSpritesheet.get();
    }

    Texture& Resources::getPalette() {
        return *paletteTexture.get();
    }

}
