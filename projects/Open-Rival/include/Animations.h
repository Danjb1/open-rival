#pragma once

#include <cstdint>
#include <map>

#include "Building.h"
#include "Unit.h"

namespace Rival {

enum class UnitAnimationType : std::uint8_t
{
    Standing,
    StandingWithBag,
    Moving,
    MovingWithBag,
    Attacking,
    Harvesting,
    Dying
};

enum class BuildingAnimationType : std::uint8_t
{
    Constructing,
    Built
};

struct Animation
{
    static constexpr int defaultMsPerFrame = 60;

    /** First sprite index of the aniamtion. */
    int startIndex;

    /** Last sprite index of the aniamtion. */
    int endIndex;

    /** How long to show each sprite index, in milliseconds. */
    int msPerFrame;

    /** Sprite index offset between different facings. */
    int facingStride;

    Animation(int startIndex, int endIndex, int msPerFrame, int facingStride);
};

using UnitAnimationPair = std::pair<Unit::Type, UnitAnimationType>;
using BuildingAnimationPair = std::pair<Building::Type, BuildingAnimationType>;
using ObjectVariantPair = std::pair<std::uint8_t, std::uint8_t>;

/*
 * Building animation definitions:
 *
static const std::map<BuildingAnimationPair, const Animation> buildingAnimationLookup = {
    // Elf
    { { Building::Type::ElvenKeep, BuildingAnimationType::Constructing }, { 0, 1, 60 } },
    { { Building::Type::ElvenKeep, BuildingAnimationType::Built }, { 2, 2, 60 } },
    { { Building::Type::Treasury, BuildingAnimationType::Constructing }, { 3, 4, 60 } },
    { { Building::Type::Treasury, BuildingAnimationType::Built }, { 5, 11, 60 } },
    { { Building::Type::CombatCamp, BuildingAnimationType::Constructing }, { 12, 13, 60 } },
    { { Building::Type::CombatCamp, BuildingAnimationType::Built }, { 14, 14, 60 } },
    { { Building::Type::Arsenal, BuildingAnimationType::Constructing }, { 15, 16, 60 } },
    { { Building::Type::Arsenal, BuildingAnimationType::Built }, { 17, 17, 60 } },
    { { Building::Type::DuelRange, BuildingAnimationType::Constructing }, { 18, 19, 60 } },
    { { Building::Type::DuelRange, BuildingAnimationType::Built }, { 20, 20, 60 } },
    { { Building::Type::HolyNest, BuildingAnimationType::Constructing }, { 21, 22, 60 } },
    { { Building::Type::HolyNest, BuildingAnimationType::Built }, { 23, 23, 60 } },
    { { Building::Type::MinerGuildhall, BuildingAnimationType::Constructing }, { 24, 25, 60 } },
    { { Building::Type::MinerGuildhall, BuildingAnimationType::Built }, { 26, 26, 60 } },
    { { Building::Type::AbbeyTower, BuildingAnimationType::Constructing }, { 27, 28, 60 } },
    { { Building::Type::AbbeyTower, BuildingAnimationType::Built }, { 29, 29, 60 } },
    { { Building::Type::CouncilOfRunes, BuildingAnimationType::Constructing }, { 30, 31, 60 } },
    { { Building::Type::CouncilOfRunes, BuildingAnimationType::Built }, { 32, 41, 60 } },
    { { Building::Type::Harbour, BuildingAnimationType::Constructing }, { 42, 43, 60 } },
    { { Building::Type::Harbour, BuildingAnimationType::Built }, { 44, 44, 60 } },
    { { Building::Type::WarningTower, BuildingAnimationType::Constructing }, { 45, 46, 60 } },
    { { Building::Type::WarningTower, BuildingAnimationType::Built }, { 47, 47, 60 } },

    // Greenskin
    { { Building::Type::Fortress, BuildingAnimationType::Constructing }, { 0, 1, 60 } },
    { { Building::Type::Fortress, BuildingAnimationType::Built }, { 2, 2, 60 } },
    { { Building::Type::HoardKeep, BuildingAnimationType::Constructing }, { 3, 4, 60 } },
    { { Building::Type::HoardKeep, BuildingAnimationType::Built }, { 5, 5, 60 } },
    { { Building::Type::Fort, BuildingAnimationType::Constructing }, { 6, 7, 60 } },
    { { Building::Type::Fort, BuildingAnimationType::Built }, { 8, 8, 60 } },
    { { Building::Type::Blacksmith, BuildingAnimationType::Constructing }, { 9, 10, 60 } },
    { { Building::Type::Blacksmith, BuildingAnimationType::Built }, { 11, 11, 60 } },
    { { Building::Type::BattleQuarters, BuildingAnimationType::Constructing }, { 12, 13, 60 } },
    { { Building::Type::BattleQuarters, BuildingAnimationType::Built }, { 14, 14, 60 } },
    { { Building::Type::BlackNest, BuildingAnimationType::Constructing }, { 15, 16, 60 } },
    { { Building::Type::BlackNest, BuildingAnimationType::Built }, { 17, 17, 60 } },
    { { Building::Type::WeirdWorkshop, BuildingAnimationType::Constructing }, { 18, 19, 60 } },
    { { Building::Type::WeirdWorkshop, BuildingAnimationType::Built }, { 20, 20, 60 } },
    { { Building::Type::UnholyChapel, BuildingAnimationType::Constructing }, { 21, 22, 60 } },
    { { Building::Type::UnholyChapel, BuildingAnimationType::Built }, { 23, 23, 60 } },
    { { Building::Type::AltarOfDoom, BuildingAnimationType::Constructing }, { 24, 25, 60 } },
    { { Building::Type::AltarOfDoom, BuildingAnimationType::Built }, { 26, 26, 60 } },
    { { Building::Type::Docks, BuildingAnimationType::Constructing }, { 27, 28, 60 } },
    { { Building::Type::Docks, BuildingAnimationType::Built }, { 29, 29, 60 } },
    { { Building::Type::GuardTower, BuildingAnimationType::Constructing }, { 30, 31, 60 } },
    { { Building::Type::GuardTower, BuildingAnimationType::Built }, { 32, 32, 60 } },

    // Human
    { { Building::Type::Castle, BuildingAnimationType::Constructing }, { 0, 1, 60 } },
    { { Building::Type::Castle, BuildingAnimationType::Built }, { 2, 2, 60 } },
    { { Building::Type::GoldMill, BuildingAnimationType::Constructing }, { 3, 4, 60 } },
    { { Building::Type::GoldMill, BuildingAnimationType::Built }, { 5, 5, 60 } },
    { { Building::Type::ArcheryRange, BuildingAnimationType::Constructing }, { 6, 7, 60 } },
    { { Building::Type::ArcheryRange, BuildingAnimationType::Built }, { 8, 8, 60 } },
    { { Building::Type::Armoury, BuildingAnimationType::Constructing }, { 9, 10, 60 } },
    { { Building::Type::Armoury, BuildingAnimationType::Built }, { 11, 11, 60 } },
    { { Building::Type::Barracks, BuildingAnimationType::Constructing }, { 12, 13, 60 } },
    { { Building::Type::Barracks, BuildingAnimationType::Built }, { 14, 14, 60 } },
    { { Building::Type::HolyStables, BuildingAnimationType::Constructing }, { 15, 16, 60 } },
    { { Building::Type::HolyStables, BuildingAnimationType::Built }, { 17, 17, 60 } },
    { { Building::Type::FireGuild, BuildingAnimationType::Constructing }, { 18, 19, 60 } },
    { { Building::Type::FireGuild, BuildingAnimationType::Built }, { 20, 20, 60 } },
    { { Building::Type::Temple, BuildingAnimationType::Constructing }, { 21, 22, 60 } },
    { { Building::Type::Temple, BuildingAnimationType::Built }, { 23, 32, 60 } },
    { { Building::Type::MageTower, BuildingAnimationType::Constructing }, { 33, 34, 60 } },
    { { Building::Type::MageTower, BuildingAnimationType::Built }, { 35, 35, 60 } },
    { { Building::Type::Shipyard, BuildingAnimationType::Constructing }, { 36, 37, 60 } },
    { { Building::Type::Shipyard, BuildingAnimationType::Built }, { 38, 38, 60 } },
    { { Building::Type::WatchTower, BuildingAnimationType::Constructing }, { 39, 40, 60 } },
    { { Building::Type::WatchTower, BuildingAnimationType::Built }, { 41, 41, 60 } }
};
*/

/*
 * Object animation definitions:
 *
 static const std::map<ObjectVariantPair, const Animation> objectAnimationLookup = {
    // Trees
    { { std::uint8_t(0x03), std::uint8_t(0) }, { 0, 5, 60 } },
    { { std::uint8_t(0x03), std::uint8_t(1) }, { 6, 11, 60 } },
    { { std::uint8_t(0x03), std::uint8_t(2) }, { 12, 17, 60 } },
    { { std::uint8_t(0x03), std::uint8_t(3) }, { 18, 23, 60 } },

    // Soft Mountains
    { { std::uint8_t(0x05), std::uint8_t(0) }, { 24, 24, 60 } },
    { { std::uint8_t(0x05), std::uint8_t(1) }, { 25, 25, 60 } },
    { { std::uint8_t(0x05), std::uint8_t(2) }, { 26, 26, 60 } },
    { { std::uint8_t(0x05), std::uint8_t(3) }, { 27, 27, 60 } },

    // Hard Mountains
    { { std::uint8_t(0x06), std::uint8_t(0) }, { 28, 28, 60 } },
    { { std::uint8_t(0x06), std::uint8_t(1) }, { 29, 29, 60 } },
    { { std::uint8_t(0x06), std::uint8_t(2) }, { 30, 30, 60 } },
    { { std::uint8_t(0x06), std::uint8_t(3) }, { 31, 31, 60 } },

    // Ornamental Objects
    { { std::uint8_t(0xAF), std::uint8_t(0) }, { 6, 6, 60 } },    // Silver pointed rock
    { { std::uint8_t(0xAF), std::uint8_t(1) }, { 7, 7, 60 } },    // Red bush
    { { std::uint8_t(0xAF), std::uint8_t(2) }, { 8, 8, 60 } },    // Silver rounded rock
    { { std::uint8_t(0xAF), std::uint8_t(3) }, { 9, 9, 60 } },    // Dark bush
    { { std::uint8_t(0xAF), std::uint8_t(4) }, { 10, 10, 60 } },  // Mossy rock
    { { std::uint8_t(0xAF), std::uint8_t(5) }, { 11, 11, 60 } },  // Dead tree
    { { std::uint8_t(0xAF), std::uint8_t(6) }, { 12, 12, 60 } },  // Stone pillar
    { { std::uint8_t(0xAF), std::uint8_t(7) }, { 13, 13, 60 } },  // Beige rock
};
*/

}  // namespace Rival
