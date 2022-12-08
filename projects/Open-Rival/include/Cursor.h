#pragma once

namespace Rival {

struct PlayerContext;

struct CursorDef
{
public:
    int startIndex;
    int endIndex;
};

namespace Cursor {

static constexpr CursorDef arrow = { 0, 7 };
static constexpr CursorDef move = { 8, 11 };
static constexpr CursorDef scrollUp = { 12, 12 };
static constexpr CursorDef scrollUpLeft = { 13, 13 };
static constexpr CursorDef scrollLeft = { 14, 14 };
static constexpr CursorDef scrollDownLeft = { 15, 15 };
static constexpr CursorDef scrollDown = { 16, 16 };
static constexpr CursorDef scrollDownRight = { 17, 17 };
static constexpr CursorDef scrollRight = { 18, 18 };
static constexpr CursorDef scrollUpRight = { 19, 19 };
static constexpr CursorDef scrollCenter = { 20, 20 };
static constexpr CursorDef attack = { 21, 24 };
static constexpr CursorDef repair = { 25, 25 };
static constexpr CursorDef select = { 26, 31 };
static constexpr CursorDef targetInvalid = { 32, 32 };
static constexpr CursorDef takeItem = { 33, 33 };
static constexpr CursorDef harvest = { 34, 37 };
static constexpr CursorDef deposit = { 38, 41 };
static constexpr CursorDef inspectContainer = { 42, 42 };
static constexpr CursorDef board = { 43, 46 };
static constexpr CursorDef targetValid = { 47, 50 };

CursorDef getCurrentCursor(const PlayerContext& playerContext);

}  // namespace Cursor

}  // namespace Rival
