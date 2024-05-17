#pragma once

#include <cstdint>

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
    Building,
    Built
};

struct Animation
{
    static constexpr int defaultMsPerFrame = 60;

    /** First sprite index of the animation. */
    int startIndex;

    /** Last sprite index of the animation. */
    int endIndex;

    /** How long to show each sprite index, in milliseconds. */
    int msPerFrame;

    /** Sprite index offset between different facings. */
    int facingStride;

    Animation(int startIndex, int endIndex, int msPerFrame, int facingStride);
};

/*
 * Object animation definitions:
 *
using ObjectVariantPair = std::pair<std::uint8_t, std::uint8_t>;

static const std::map<ObjectVariantPair, const Animation> objectAnimationLookup = {
    // Trees
    { { std::uint8_t(0x03), std::uint8_t(0) }, { 0, 5, 60, 0 } },
    { { std::uint8_t(0x03), std::uint8_t(1) }, { 6, 11, 60, 0 } },
    { { std::uint8_t(0x03), std::uint8_t(2) }, { 12, 17, 60, 0 } },
    { { std::uint8_t(0x03), std::uint8_t(3) }, { 18, 23, 60, 0 } },

    // Soft Mountains
    { { std::uint8_t(0x05), std::uint8_t(0) }, { 24, 24, 60, 0 } },
    { { std::uint8_t(0x05), std::uint8_t(1) }, { 25, 25, 60, 0 } },
    { { std::uint8_t(0x05), std::uint8_t(2) }, { 26, 26, 60, 0 } },
    { { std::uint8_t(0x05), std::uint8_t(3) }, { 27, 27, 60, 0 } },

    // Hard Mountains
    { { std::uint8_t(0x06), std::uint8_t(0) }, { 28, 28, 60, 0 } },
    { { std::uint8_t(0x06), std::uint8_t(1) }, { 29, 29, 60, 0 } },
    { { std::uint8_t(0x06), std::uint8_t(2) }, { 30, 30, 60, 0 } },
    { { std::uint8_t(0x06), std::uint8_t(3) }, { 31, 31, 60, 0 } },

    // Ornamental Objects
    { { std::uint8_t(0xAF), std::uint8_t(0) }, { 6, 6, 60, 0 } },    // Silver pointed rock
    { { std::uint8_t(0xAF), std::uint8_t(1) }, { 7, 7, 60, 0 } },    // Red bush
    { { std::uint8_t(0xAF), std::uint8_t(2) }, { 8, 8, 60, 0 } },    // Silver rounded rock
    { { std::uint8_t(0xAF), std::uint8_t(3) }, { 9, 9, 60, 0 } },    // Dark bush
    { { std::uint8_t(0xAF), std::uint8_t(4) }, { 10, 10, 60, 0 } },  // Mossy rock
    { { std::uint8_t(0xAF), std::uint8_t(5) }, { 11, 11, 60, 0 } },  // Dead tree
    { { std::uint8_t(0xAF), std::uint8_t(6) }, { 12, 12, 60, 0 } },  // Stone pillar
    { { std::uint8_t(0xAF), std::uint8_t(7) }, { 13, 13, 60, 0 } },  // Beige rock
};
*/

}  // namespace Rival
