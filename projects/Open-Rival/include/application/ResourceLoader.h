#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "audio/Sounds.h"
#include "game/BuildingType.h"
#include "game/UnitType.h"
#include "gfx/Font.h"
#include "gfx/Spritesheet.h"

namespace Rival {

class ApplicationContext;
class AttackDef;
class BuildingDef;
class EffectDef;
class MidiFile;
class ProjectileDef;
class Resources;
class Texture;
class TextureAtlas;
class TextureStore;
class UnitDef;
class WaveFile;

/** Loads the game's resources into a Resources object. */
class ResourceLoader
{
public:
    ResourceLoader(const ApplicationContext& context, Resources& resources);

private:
    // Fonts
    std::shared_ptr<const Font> initFontSmall(const ApplicationContext& context);
    std::shared_ptr<const Font> initFontRegular(const ApplicationContext& context);

    // Textures
    std::vector<std::shared_ptr<const Texture>> loadTextures();
    std::vector<std::shared_ptr<const TextureAtlas>> loadTextureAtlases();
    std::shared_ptr<const Texture> initPaletteTexture();

    // Spritesheets
    std::unordered_map<BuildingType, std::shared_ptr<const Spritesheet>> initBuildingSpritesheets(
            const TextureStore& textureStore);
    std::unordered_map<UnitType, std::shared_ptr<const Spritesheet>> initUnitSpritesheets(
            const TextureStore& textureStore);
    std::unordered_map<std::string, std::shared_ptr<const Spritesheet>> initProjectileSpritesheets(
            const TextureStore& textureStore);
    std::vector<std::shared_ptr<const Spritesheet>> initTileSpritesheets(const TextureStore& textureStore);
    std::vector<std::shared_ptr<const Spritesheet>> initObjectSpritesheets(const TextureStore& textureStore);
    std::shared_ptr<const Spritesheet> initCursorSpritesheet(const TextureStore& textureStore);
    std::shared_ptr<const Spritesheet> initMapBorderSpritesheet(const TextureStore& textureStore);
    std::shared_ptr<const Spritesheet> initPortraitSpritesheet(const TextureStore& textureStore);
    std::shared_ptr<const Spritesheet> initHitboxSpritesheet(const TextureStore& textureStore);

    // Sounds
    std::vector<std::shared_ptr<const WaveFile>> initSounds();
    std::unordered_map<std::string, const SoundBank> initSoundBanks() const;

    // MIDIs
    std::vector<std::shared_ptr<const MidiFile>> initMidis();

    // Data
    std::unordered_map<UnitType, UnitDef> initUnitDefs() const;
    std::unordered_map<BuildingType, BuildingDef> initBuildingDefs() const;
    std::unordered_map<int, const AttackDef> initAttackDefs() const;
    std::unordered_map<std::string, const ProjectileDef> initProjectileDefs() const;
    std::unordered_map<std::string, const EffectDef> initEffectDefs() const;

public:
    // Directories
    static const std::string dataDir;
    static const std::string mapsDir;
    static const std::string soundDir;
    static const std::string txDir;

    // Texture indices
    static constexpr int txIndexUnits = 0;
    static constexpr int txIndexProjectiles = txIndexUnits + 50;
    static constexpr int txIndexTiles = txIndexProjectiles + 17;
    static constexpr int txIndexObjects = txIndexTiles + 4;
    static constexpr int txIndexCursors = txIndexObjects + 3;
    static constexpr int txIndexBuildings = txIndexCursors + 1;
    static constexpr int txIndexPortraits = txIndexBuildings + 3;
    static constexpr int txIndexHitbox = txIndexPortraits + 1;
    static constexpr int txIndexMenuBackground = txIndexHitbox + 1;

private:
    // Fonts
    static const std::vector<std::string> defaultFontDirs;
    static const std::string defaultFontSmall;
    static const std::string defaultFontRegular;

    // Textures
    static constexpr int numTextures = txIndexMenuBackground + 1;
    static constexpr int numTextureAtlases = 2;

    // Sounds
    static constexpr int numSounds = 369;
    static constexpr int numMidis = 1;  // 13;
    static constexpr int midiStartIndex = 369;
};

}  // namespace Rival
