#pragma once

namespace Rival {

class Rect
{

public:
    float x;
    float y;
    float width;
    float height;

    Rect(int x, int y, int width, int height);
    Rect(float x, float y, float width, float height);

    bool contains(int x, int y) const;
    bool contains(float x, float y) const;
};

}  // namespace Rival
