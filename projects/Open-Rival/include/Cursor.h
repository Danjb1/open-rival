#pragma once

namespace Rival {

class PlayerStore;
struct PlayerContext;

struct CursorDef
{
public:
    int startIndex;
    int endIndex;
    float hotspotX;
    float hotspotY;

    bool operator==(const CursorDef& other) const
    {
        // We can assume that 2 CursorDefs with the same startIndex are, in fact, the same cursor
        return startIndex == other.startIndex;
    }

    bool operator!=(const CursorDef& other) const
    {
        return !(*this == other);
    }
};

namespace Cursor {

static constexpr CursorDef arrow = { 0, 7, 0.f, 0.f };
static constexpr CursorDef move = { 8, 11, 0.5f, 0.5f };
static constexpr CursorDef scrollUp = { 12, 12, 0.f, 0.f };
static constexpr CursorDef scrollUpLeft = { 13, 13, 0.f, 0.f };
static constexpr CursorDef scrollLeft = { 14, 14, 0.f, 0.f };
static constexpr CursorDef scrollDownLeft = { 15, 15, 0.f, 0.f };
static constexpr CursorDef scrollDown = { 16, 16, 0.f, 0.f };
static constexpr CursorDef scrollDownRight = { 17, 17, 0.f, 0.f };
static constexpr CursorDef scrollRight = { 18, 18, 0.f, 0.f };
static constexpr CursorDef scrollUpRight = { 19, 19, 0.f, 0.f };
static constexpr CursorDef scrollCenter = { 20, 20, 0.f, 0.f };
static constexpr CursorDef attack = { 21, 24, 0.5f, 0.5f };
static constexpr CursorDef repair = { 25, 25, 0.5f, 0.5f };
static constexpr CursorDef select = { 26, 31, 0.5f, 0.5f };
static constexpr CursorDef targetInvalid = { 32, 32, 0.5f, 0.5f };
static constexpr CursorDef takeItem = { 33, 33, 0.f, 0.f };
static constexpr CursorDef harvest = { 34, 37, 0.f, 0.f };
static constexpr CursorDef deposit = { 38, 41, 0.f, 0.f };
static constexpr CursorDef inspectContainer = { 42, 42, 0.f, 0.f };
static constexpr CursorDef board = { 43, 46, 0.f, 0.f };
static constexpr CursorDef targetValid = { 47, 50, 0.5f, 0.5f };

CursorDef getCurrentCursor(const PlayerStore& playerStore, const PlayerContext& playerContext);

}  // namespace Cursor

}  // namespace Rival
