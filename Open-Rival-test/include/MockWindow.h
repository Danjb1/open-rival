#ifndef WINDOW_H
#define WINDOW_H

namespace Rival {

    class Window {

    public:
        Window(int w, int h);

        void swapBuffers() const;

        int getWidth() const;

        int getHeight() const;

        double getAspectRatio() const;

    private:
        int width;
        int height;
    };

}  // namespace Rival

#endif  // WINDOW_H
