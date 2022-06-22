#pragma once

namespace Rival {

class Rect
{

public:
    Rect(int x, int y, int width, int height);
    Rect(float x, float y, float width, float height);

    bool contains(int x, int y) const;
    bool contains(float x, float y) const;

public:
    float x;
    float y;
    float width;
    float height;
};

}  // namespace Rival
