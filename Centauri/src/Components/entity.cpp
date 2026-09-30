
#include "entity.h"

namespace Centauri {
    void Entity::Initialize() {
        for (auto& component: components) {
            component -> Initialize();
        }
    }

    void Entity::Update(float deltaTime) {
        if (!active) return;

        for (auto& component: components) {
            component -> Update(deltaTime);
        }
    }

    void Entity::Render() {
        if (!active) return;

        for (auto& component: components) {
            component -> Render();
        }
    }

    template<typename T, typename... Args>
    T* Entity::AddComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "The object must derive from Component!");

        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* componentPtr = component.get();
        componentPtr->SetOwner(this);
        components.push_back(std::move(component));
        return componentPtr;
    }

    template<typename T>
    bool Entity::RemoveComponent() {
        for (auto it = components.begin(); it != components.end(); ++it) {
            if (dynamic_cast<T*>(it -> get())) {
                components.erase(it);
                return true;
            }   
        }
        return false;
        
    }

    template<typename T>
    T* Entity::GetComponent() {
        for (auto& component : components) {
            if (T* result = dynamic_cast<T*>(component.get())) {
                return result;
            }
        }
        return nullptr;
    } 
}