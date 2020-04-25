#ifndef MOUSE_PICKER_H
#define MOUSE_PICKER_H

#include "Camera.h"

namespace Rival {

    class MousePicker {

    public:

        MousePicker(Camera& camera, int mapWidth, int mapHeight);

        void handleMouse();

        int getTileX() const;

        int getTileY() const;

    private:

        Camera& camera;

        int mapWidth;

        int mapHeight;

        int tileX;

        int tileY;

    };

}

#endif // MOUSE_PICKER_H
