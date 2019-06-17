#ifndef SCENE_H
#define SCENE_H

#include <map>

#include "Unit.h"

namespace Rival {

    class Scene {

    private:

        std::map<int, std::unique_ptr<Unit>> units;

    public:

        Scene();

        void addUnit(std::unique_ptr<Unit> unit);

        std::map<int, std::unique_ptr<Unit>>& getUnits();

    };

}

#endif // SCENE_H
