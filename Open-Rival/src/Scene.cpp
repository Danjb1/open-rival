#include "pch.h"
#include "Scene.h"

namespace Rival {

    Scene::Scene() {}

    void Scene::addUnit(std::unique_ptr<Unit> unit) {
        units[0] = std::move(unit);
    }

    std::map<int, std::unique_ptr<Unit>>& Scene::getUnits() {
        return units;
    }

}
