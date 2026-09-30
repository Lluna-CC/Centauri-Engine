#ifndef ENTITY_H
#define ENTITY_H

#include "component.h"

namespace Centauri {
    class Entity {
      public:
        explicit Entity(const std::string& entityName) : name(entityName) {}      
        
        const std::string& GetName() const {return name;}
        void Initialize();
        void Update(float deltaTimne);
        void Render();

        template<typename T, typename... Args>
        T* AddComponent(Args&&... args);

        template<typename T>
        bool RemoveComponent();

        template<typename T>
        T* GetComponent();        


      private:
        std::string name;
        std::vector<std::unique_ptr<Component>> components;
    }
}

#endif //ENTITY_H