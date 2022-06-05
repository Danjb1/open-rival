#ifndef COLOR_H
#define COLOR_H

namespace Rival {

    class Color
    {
    public:
        float r;
        float g;
        float b;
        float a;

        /**
         * Factory method for creating a Color.
         *
         * Parameters should be in the range 0-255.
         */
        static Color makeRgb(int r, int g, int b);

    private:
        Color(float r, float g, float b);
    };

}  // namespace Rival

#endif  // COLOR_H
