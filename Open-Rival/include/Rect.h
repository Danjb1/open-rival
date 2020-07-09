#ifndef RECT_H
#define RECT_H

namespace Rival {

    class Rect {

    public:
        float x;
        float y;
        float width;
        float height;

        Rect(float x, float y, float width, float height);

        bool contains(int x, int y) const;
        bool contains(float x, float y) const;
    };

}  // namespace Rival

#endif  // RECT_H
