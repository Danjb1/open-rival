#ifndef MOUSE_PICKER_H
#define MOUSE_PICKER_H

#include <memory>

#include "Camera.h"

namespace Rival {

    class MousePicker {

    public:

        MousePicker(Camera& camera, std::shared_ptr<Scenario> scenario);

        void handleMouse();

        int getTileX() const;

        int getTileY() const;

        int getEntity() const;
    private:

        Camera& camera;

        int mapWidth;

        int mapHeight;

        int tileX;

        int tileY;

        int entity;

        std::shared_ptr<Scenario> scenario;
    };

}

#endif // MOUSE_PICKER_H
