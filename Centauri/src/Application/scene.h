#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "../Components/entity.h"

namespace Centauri {
    class Scene {
      public:
        Scene() = default;
        ~Scene() = default;

        void Update(float deltaTime);
        void Render();

        //Function arguments might change in the future
        Entity* AddEntity();

        bool RemoveEntity(std::string name);
        Entity* GetEntity(std::string name);

      private:

        //Vector or map?
        std::unordered_map<std::string, std::unique_ptr<Entity>> entities;
    };
}

#endif //SCENE_H