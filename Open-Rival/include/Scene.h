#ifndef SCENE_H
#define SCENE_H

#include <map>

#include "Unit.h"

namespace Rival {

    class Scene {

    public:

        Scene();

        void addUnit(std::unique_ptr<Unit> unit);

        std::map<int, std::unique_ptr<Unit>>& getUnits();

    private:

        int nextId;

        std::map<int, std::unique_ptr<Unit>> units;

    };

}

#endif // SCENE_H
