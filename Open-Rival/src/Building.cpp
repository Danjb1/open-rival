#include "pch.h"
#include "Building.h"

namespace Rival {
namespace Building {

    int getWidth(Type type) {
        return isWall(type) ? wallWidth : defaultWidth;
    }

    int getHeight(Type type) {
        return isWall(type) ? wallHeight : defaultHeight;
    }

    bool isWall(Type type) {
        return type == Type::Wall
                || type == Type::TreeWall
                || type == Type::GreenskinWall;
    }

}}  // namespace Rival::Building
