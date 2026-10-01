#include "scene.h"

namespace Centauri {
    void Scene::Update(float deltaTime) {
        for (const auto& [name,entity] : entities) {
            entity -> Update(deltaTime); 
        }
    }

    void Scene::Render() {
        //Initialize rendering 

        for (const auto& [name,entity] : entities) {
            entity -> Render(); 
        }
    }
}

