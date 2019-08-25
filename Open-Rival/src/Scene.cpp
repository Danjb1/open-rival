#include "pch.h"
#include "Scene.h"

namespace Rival {

    Scene::Scene() {}

    void Scene::addUnit(std::unique_ptr<Unit> unit) {
        units[nextId] = std::move(unit);
        units[nextId]->addedToScene(nextId);
        nextId++;
    }

    std::map<int, std::unique_ptr<Unit>>& Scene::getUnits() {
        return units;
    }

}
