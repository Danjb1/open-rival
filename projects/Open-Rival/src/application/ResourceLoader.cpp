#include "application/ResourceLoader.h"

#include "application/ApplicationContext.h"
#include "application/PathUtils.h"
#include "application/Resources.h"
#include "audio/MidiFile.h"
#include "audio/MidsDecoder.h"
#include "audio/WaveFile.h"
#include "game/AttackDef.h"
#include "game/BuildingDef.h"
#include "game/GameInterface.h"
#include "game/UnitDef.h"
#include "gfx/PaletteUtils.h"
#include "gfx/RenderUtils.h"
#include "gfx/Texture.h"
#include "gfx/TextureAtlas.h"
#include "utils/ConfigUtils.h"
#include "utils/FileUtils.h"
#include "utils/LogUtils.h"

namespace Rival {

const std::string ResourceLoader::dataDir = "res\\data\\";
const std::string ResourceLoader::mapsDir = "res\\maps\\";
const std::string ResourceLoader::soundDir = "res\\sound\\";
const std::string ResourceLoader::txDir = "res\\textures\\";

const std::vector<std::string> ResourceLoader::defaultFontDirs = PathUtils::getDefaultFontDirs();
const std::string ResourceLoader::defaultFontSmall = "serife.fon";
const std::string ResourceLoader::defaultFontRegular = "Procopius Regular.ttf";

const std::vector<std::string> textureNames = {
    // Units - Human
    "unit_human_ballista.tga",
    "unit_human_battleship.tga",
    "unit_human_bowman.tga",
    "unit_human_chariot_of_war.tga",
    "unit_human_fire_master.tga",
    "unit_human_knight.tga",
    "unit_human_light_cavalry.tga",
    "unit_human_peasant.tga",
    "unit_human_pegas_rider.tga",
    "unit_human_priest.tga",
    "unit_human_sea_barge.tga",
    "unit_human_thief.tga",
    "unit_human_wizard.tga",
    "unit_human_zeppelin.tga",

    // Units - Greenskin
    "unit_greenskin_balloon.tga",
    "unit_greenskin_catapult.tga",
    "unit_greenskin_gnome_boomer.tga",
    "unit_greenskin_horde_rider.tga",
    "unit_greenskin_landing_craft.tga",
    "unit_greenskin_necromancer.tga",
    "unit_greenskin_priest_of_doom.tga",
    "unit_greenskin_rock_thrower.tga",
    "unit_greenskin_rogue.tga",
    "unit_greenskin_serf.tga",
    "unit_greenskin_storm_trooper.tga",
    "unit_greenskin_troll_galley.tga",
    "unit_greenskin_warbat.tga",
    "unit_greenskin_warlord.tga",

    // Units - Elf
    "unit_elf_archer.tga",
    "unit_elf_arquebusier.tga",
    "unit_elf_bark.tga",
    "unit_elf_bombard.tga",
    "unit_elf_centaur.tga",
    "unit_elf_druid.tga",
    "unit_elf_dwarf_miner.tga",
    "unit_elf_enchanter.tga",
    "unit_elf_mage.tga",
    "unit_elf_magic_chopper.tga",
    "unit_elf_scout.tga",
    "unit_elf_sky_rider.tga",
    "unit_elf_warship.tga",
    "unit_elf_yeoman.tga",

    // Units - Monsters
    "unit_monster_devil.tga",
    "unit_monster_dragon.tga",
    "unit_monster_golem.tga",
    "unit_monster_gryphon.tga",
    "unit_monster_hydra.tga",
    "unit_monster_sea_monster.tga",
    "unit_monster_skeleton.tga",
    "unit_monster_snake.tga",

    // Projectiles
    "projectile_arrow.tga",
    "projectile_ballista.tga",
    "projectile_boulder.tga ",
    "projectile_cannonball.tga",
    "projectile_fire_arrow.tga",
    "projectile_fireball.tga",
    "projectile_flame_ring.tga",
    "projectile_flash_light_elf1.tga",
    "projectile_flash_light_elf2.tga",
    "projectile_flash_light_greenskin.tga",
    "projectile_flash_light_human.tga",
    "projectile_gun.tga",
    "projectile_mace.tga",
    "projectile_pegas_3arrow.tga",
    "projectile_pegas_arrow.tga",
    "projectile_poison_spit.tga",
    "projectile_rock.tga",

    // Tiles
    "tiles_meadow.tga",
    "tiles_wilderness.tga",
    "tiles_fog.tga",
    "tiles_map_border.tga",

    // Objects
    "objects_common.tga",
    "objects_meadow.tga",
    "objects_wilderness.tga",

    // Cursors
    "game_cursors.tga",

    // Buildings
    "buildings_elf.tga",
    "buildings_greenskin.tga",
    "buildings_human.tga",

    // UI
    "portraits.tga",

    // Hitbox
    "hitbox.tga",

    // Menu Background
    "menu_bg_dark.tga",
};

ResourceLoader::ResourceLoader(const ApplicationContext& context, Resources& resources)
{
    // For now we can set these properties directly because we are a friend class.
    // Later, Resources should expose public methods to register fonts, textures, spritesheets, etc.
    resources.fontSmall = initFontSmall(context);
    resources.fontRegular = initFontRegular(context);
    resources.textures = loadTextures();
    resources.textureAtlases = loadTextureAtlases();
    resources.paletteTexture = initPaletteTexture();
    resources.unitSpritesheets = initUnitSpritesheets(resources);
    resources.projectileSpritesheets = initProjectileSpritesheets(resources);
    resources.buildingSpritesheets = initBuildingSpritesheets(resources);
    resources.tileSpritesheets = initTileSpritesheets(resources);
    resources.objectSpritesheets = initObjectSpritesheets(resources);
    resources.cursorSpritesheet = initCursorSpritesheet(resources);
    resources.mapBorderSpritesheet = initMapBorderSpritesheet(resources);
    resources.portraitSpritesheet = initPortraitSpritesheet(resources);
    resources.hitboxSpritesheet = initHitboxSpritesheet(resources);
    resources.sounds = initSounds();
    resources.midis = initMidis();
    resources.unitDefs = initUnitDefs();
    resources.buildingDefs = initBuildingDefs();
    resources.attackDefs = initAttackDefs();
    resources.projectileDefs = initProjectileDefs();
    resources.soundBanks = initSoundBanks();
}

std::shared_ptr<const Font> ResourceLoader::initFontSmall(const ApplicationContext& context)
{
    const json& cfg = context.getConfig();
    std::vector<std::string> fontDirs = ConfigUtils::get(cfg, "fontDirs", defaultFontDirs);
    std::string fontName = ConfigUtils::get(cfg, "fontSmall", defaultFontSmall);
    int fontSize = ConfigUtils::get(cfg, "fontSmallSize", 32);
    return Font::loadFont(context.getFontLibrary(), fontDirs, fontName, fontSize);
}

std::shared_ptr<const Font> ResourceLoader::initFontRegular(const ApplicationContext& context)
{
    const json& cfg = context.getConfig();
    std::vector<std::string> fontDirs = ConfigUtils::get(cfg, "fontDirs", defaultFontDirs);
    std::string fontName = ConfigUtils::get(cfg, "fontRegular", defaultFontRegular);
    int fontSize = ConfigUtils::get(cfg, "fontRegularSize", 16);
    return Font::loadFont(context.getFontLibrary(), fontDirs, fontName, fontSize);
}

std::vector<std::shared_ptr<const Texture>> ResourceLoader::loadTextures()
{
    std::vector<std::shared_ptr<const Texture>> texList;
    texList.reserve(numTextures);

    for (const auto& textureName : textureNames)
    {
        texList.push_back(Texture::loadTexture(txDir + textureName));
    }

    return texList;
}

std::vector<std::shared_ptr<const TextureAtlas>> ResourceLoader::loadTextureAtlases()
{
    std::vector<std::shared_ptr<const TextureAtlas>> texAtlasList;
    texAtlasList.reserve(numTextureAtlases);

    std::vector<std::string> resourceNames = { "game_interface", "overlays" };

    for (const auto& resourceName : resourceNames)
    {
        texAtlasList.push_back(TextureAtlas::loadTextureAtlas(txDir + resourceName));
    }

    return texAtlasList;
}

std::shared_ptr<const Texture> ResourceLoader::initPaletteTexture()
{
    return PaletteUtils::createPaletteTexture();
}

std::unordered_map<BuildingType, std::shared_ptr<const Spritesheet>> ResourceLoader::initBuildingSpritesheets(
        const TextureStore& textureStore)
{
    std::unordered_map<BuildingType, std::shared_ptr<const Spritesheet>> spritesheets;
    int nextIndex = txIndexBuildings;

    auto createSpritesheets = [&](BuildingType first, BuildingType last) {
        for (int i = static_cast<int>(first); i <= static_cast<int>(last); ++i)
        {
            BuildingType buildingType = static_cast<BuildingType>(i);
            auto spritesheet = std::make_shared<const Spritesheet>(
                    textureStore.getTexture(nextIndex), RenderUtils::entityWidthPx, RenderUtils::entityHeightPx, 0);
            spritesheets.insert({ buildingType, spritesheet });
        }
    };

    createSpritesheets(BuildingType::ElfFirst, BuildingType::ElfLast);
    ++nextIndex;
    createSpritesheets(BuildingType::GreenskinFirst, BuildingType::GreenskinLast);
    ++nextIndex;
    createSpritesheets(BuildingType::HumanFirst, BuildingType::HumanLast);

    return spritesheets;
}

std::unordered_map<UnitType, std::shared_ptr<const Spritesheet>> ResourceLoader::initUnitSpritesheets(
        const TextureStore& textureStore)
{
    std::unordered_map<UnitType, std::shared_ptr<const Spritesheet>> spritesheets;

    for (int i = static_cast<int>(UnitType::First); i <= static_cast<int>(UnitType::Last); ++i)
    {
        UnitType unitType = static_cast<UnitType>(i);
        auto spritesheet = std::make_shared<const Spritesheet>(
                textureStore.getTexture(i), RenderUtils::entityWidthPx, RenderUtils::entityHeightPx, 0);
        spritesheets.insert({ unitType, spritesheet });
    }

    return spritesheets;
}

std::unordered_map<std::string, std::shared_ptr<const Spritesheet>> ResourceLoader::initProjectileSpritesheets(
        const TextureStore& textureStore)
{
    std::unordered_map<std::string, std::shared_ptr<const Spritesheet>> spritesheets;

    for (int i = txIndexProjectiles; i < txIndexTiles; ++i)
    {
        std::string texName = textureNames[i];
        auto spritesheet = std::make_shared<const Spritesheet>(
                textureStore.getTexture(i), RenderUtils::entityWidthPx, RenderUtils::entityHeightPx, 0);
        spritesheets.insert({ texName, spritesheet });
    }

    return spritesheets;
}

std::vector<std::shared_ptr<const Spritesheet>> ResourceLoader::initTileSpritesheets(const TextureStore& textureStore)
{
    std::vector<std::shared_ptr<const Spritesheet>> spritesheets;

    // 0 = Meadow
    // 1 = Wilderness
    // 2 = Fog
    for (int i = txIndexTiles; i < txIndexTiles + 3; i++)
    {
        auto spritesheet = std::make_shared<const Spritesheet>(
                textureStore.getTexture(i), RenderUtils::tileSpriteWidthPx, RenderUtils::tileSpriteHeightPx, 1);
        spritesheets.push_back(spritesheet);
    }

    return spritesheets;
}

std::vector<std::shared_ptr<const Spritesheet>> ResourceLoader::initObjectSpritesheets(const TextureStore& textureStore)
{
    std::vector<std::shared_ptr<const Spritesheet>> spritesheets;

    // 0 = Common
    // 1 = Meadow
    // 2 = Wilderness
    for (int i = txIndexObjects; i < txIndexObjects + 3; i++)
    {
        auto spritesheet = std::make_shared<const Spritesheet>(
                textureStore.getTexture(i), RenderUtils::entityWidthPx, RenderUtils::entityHeightPx, 0);
        spritesheets.push_back(spritesheet);
    }

    return spritesheets;
}

std::shared_ptr<const Spritesheet> ResourceLoader::initMapBorderSpritesheet(const TextureStore& textureStore)
{
    return std::make_shared<const Spritesheet>(textureStore.getTexture(txIndexTiles + 3),
            RenderUtils::tileSpriteWidthPx,
            RenderUtils::tileSpriteHeightPx,
            1);
}

std::shared_ptr<const Spritesheet> ResourceLoader::initCursorSpritesheet(const TextureStore& textureStore)
{
    return std::make_shared<const Spritesheet>(textureStore.getTexture(txIndexCursors),
            static_cast<int>(RenderUtils::cursorWidthPx),
            static_cast<int>(RenderUtils::cursorHeightPx),
            0);
}

std::shared_ptr<const Spritesheet> ResourceLoader::initPortraitSpritesheet(const TextureStore& textureStore)
{
    return std::make_shared<const Spritesheet>(textureStore.getTexture(txIndexPortraits),
            static_cast<int>(GameInterface::portrait.width),
            static_cast<int>(GameInterface::portrait.height),
            1);
}

std::shared_ptr<const Spritesheet> ResourceLoader::initHitboxSpritesheet(const TextureStore& textureStore)
{
    return std::make_shared<const Spritesheet>(textureStore.getTexture(txIndexHitbox),
            RenderUtils::hitboxSpriteWidthPx,
            RenderUtils::hitboxSpriteHeightPx,
            0);
}

std::vector<std::shared_ptr<const WaveFile>> ResourceLoader::initSounds()
{
    std::vector<std::shared_ptr<const WaveFile>> soundsRead;
    soundsRead.reserve(numSounds);

    for (int i = 0; i < numSounds; ++i)
    {
        std::string fileNum = std::to_string(i);
        std::string filename = std::string(3 - fileNum.length(), '0') + fileNum + ".wav";
        soundsRead.push_back(std::make_shared<WaveFile>(soundDir + filename));
    }

    return soundsRead;
}

std::vector<std::shared_ptr<const MidiFile>> ResourceLoader::initMidis()
{
    std::vector<std::shared_ptr<const MidiFile>> midisRead;

    for (int i = 0; i < numMidis; ++i)
    {
        midi_container result;

        const std::string midiFilename = std::to_string(midiStartIndex + i) + ".mid";
        LOG_TRACE("Reading MIDI file: {}", midiFilename);

        // Read MIDI file
        std::vector<std::uint8_t> const& p_file = FileUtils::readBinaryFile(soundDir + midiFilename);

        // Parse MIDI file
        if (!MidsDecoder::process_mids(p_file, result))
        {
            throw std::runtime_error("Failed to parse MIDI file!");
        }

        midisRead.push_back(std::make_shared<const MidiFile>(result));
    }

    return midisRead;
}

std::unordered_map<UnitType, UnitDef> ResourceLoader::initUnitDefs() const
{
    const json rawData = JsonUtils::readJsonFile(dataDir + "units.json");
    json::const_reference unitList = rawData.at("units");

    std::unordered_map<UnitType, UnitDef> allUnitDefs;
    int nextUnitType = 0;

    for (const auto& rawUnitDef : unitList)
    {
        if (nextUnitType < 0 || nextUnitType > static_cast<int>(UnitType::Last))
        {
            throw std::runtime_error("Trying to parse invalid unit type: " + std::to_string(nextUnitType));
        }

        UnitType unitType = static_cast<UnitType>(nextUnitType);

        try
        {
            allUnitDefs.emplace(unitType, UnitDef::fromJson(rawUnitDef));
        }
        catch (const json::exception&)
        {
            LOG_ERROR("Error parsing unit definition: {}", nextUnitType);
            throw;
        }

        ++nextUnitType;
    }

    return allUnitDefs;
}

std::unordered_map<BuildingType, BuildingDef> ResourceLoader::initBuildingDefs() const
{
    const json rawData = JsonUtils::readJsonFile(dataDir + "buildings.json");
    json::const_reference buildingList = rawData.at("buildings");

    std::unordered_map<BuildingType, BuildingDef> allBuildingDefs;
    int nextBuildingType = 0;

    for (const auto& rawBuildingDef : buildingList)
    {
        if (nextBuildingType < 0 || nextBuildingType > static_cast<int>(BuildingType::Last))
        {
            throw std::runtime_error("Trying to parse invalid building type: " + std::to_string(nextBuildingType));
        }

        BuildingType unitType = static_cast<BuildingType>(nextBuildingType);

        try
        {
            allBuildingDefs.emplace(unitType, BuildingDef::fromJson(rawBuildingDef));
        }
        catch (const json::exception&)
        {
            LOG_ERROR("Error parsing building definition: {}", nextBuildingType);
            throw;
        }

        ++nextBuildingType;
    }

    return allBuildingDefs;
}

std::unordered_map<int, const AttackDef> ResourceLoader::initAttackDefs() const
{
    const json rawData = JsonUtils::readJsonFile(dataDir + "attacks.json");
    json::const_reference attackList = rawData.at("attacks");

    std::unordered_map<int, const AttackDef> allAttackDefs;
    int nextAttackType = 0;

    for (const auto& rawAttackDef : attackList)
    {
        try
        {
            allAttackDefs.emplace(nextAttackType, AttackDef::fromJson(rawAttackDef));
        }
        catch (const json::exception&)
        {
            LOG_ERROR("Error parsing attack definition: {}", nextAttackType);
            throw;
        }

        ++nextAttackType;
    }

    return allAttackDefs;
}

std::unordered_map<std::string, const ProjectileDef> ResourceLoader::initProjectileDefs() const
{
    const json rawData = JsonUtils::readJsonFile(dataDir + "projectiles.json");
    json::const_reference projectileList = rawData.at("projectiles");

    std::unordered_map<std::string, const ProjectileDef> allProjectileDefs;

    for (const auto& entry : projectileList.items())
    {
        try
        {
            const std::string projectileName = entry.key();
            const auto projectileDef = ProjectileDef::fromJson(entry.value());
            allProjectileDefs.emplace(projectileName, projectileDef);
        }
        catch (const json::exception&)
        {
            LOG_ERROR("Error parsing projectile definition: {}", entry.key());
            throw;
        }
    }

    return allProjectileDefs;
}

std::unordered_map<std::string, const SoundBank> ResourceLoader::initSoundBanks() const
{
    json rawData = JsonUtils::readJsonFile(dataDir + "sounds.json");
    json::const_reference soundList = rawData.at("sounds");

    std::unordered_map<std::string, const SoundBank> allSoundBanks;

    for (json::const_iterator it = soundList.cbegin(); it != soundList.cend(); ++it)
    {
        try
        {
            const std::string soundKey = it.key();
            std::vector<int> soundIds = it.value();
            allSoundBanks.emplace(soundKey, soundIds);
        }
        catch (const json::exception&)
        {
            LOG_ERROR("Error parsing sound bank");
            throw;
        }
    }

    return allSoundBanks;
}

}  // namespace Rival
