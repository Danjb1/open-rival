#pragma once

namespace Rival {

class Rect
{

public:
    Rect() {}
    Rect(int x, int y, int width, int height);
    Rect(float x, float y, float width, float height);

    bool contains(int x, int y) const;
    bool contains(float x, float y) const;
    bool intersects(Rect other) const;

public:
    float x = 0.f;
    float y = 0.f;
    float width = 0.f;
    float height = 0.f;
};

}  // namespace Rival
