#pragma once

#include <cstdint>

namespace Rival { namespace Building {

enum class Type : std::uint8_t
{

    // Elf
    ElvenKeep,
    Treasury,
    CombatCamp,
    Arsenal,
    DuelRange,
    HolyNest,
    MinerGuildhall,
    AbbeyTower,
    CouncilOfRunes,
    Harbour,
    WarningTower,
    TreeWall,

    // Greenskin
    Fortress,
    HoardKeep,
    Fort,
    Blacksmith,
    BattleQuarters,
    BlackNest,
    WeirdWorkshop,
    UnholyChapel,
    AltarOfDoom,
    Docks,
    GuardTower,
    GreenskinWall,

    // Human
    Castle,
    GoldMill,
    ArcheryRange,
    Armoury,
    Barracks,
    HolyStables,
    FireGuild,
    Temple,
    MageTower,
    Shipyard,
    WatchTower,
    Wall,

    // Other
    Invalid
};

static constexpr int firstElfBuildingType = static_cast<int>(Type::ElvenKeep);
static constexpr int lastElfBuildingType = static_cast<int>(Type::TreeWall);
static constexpr int firstGreenskinBuildingType = static_cast<int>(Type::Fortress);
static constexpr int lastGreenskinBuildingType = static_cast<int>(Type::GreenskinWall);
static constexpr int firstHumanBuildingType = static_cast<int>(Type::Castle);
static constexpr int lastHumanBuildingType = static_cast<int>(Type::Wall);

static const int firstBuildingType = firstElfBuildingType;
static const int lastBuildingType = lastHumanBuildingType;

static constexpr int defaultWidth = 3;
static constexpr int defaultHeight = 2;
static constexpr int wallWidth = 1;
static constexpr int wallHeight = 1;

int getWidth(Type type);
int getHeight(Type type);
bool isWall(Type type);

}}  // namespace Rival::Building
