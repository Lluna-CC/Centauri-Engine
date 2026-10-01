#include "resource.h"


namespace Centauri {
    template<typename T>
    ResourceHandle<T> ResourceManager::Load(const std::string& name) {
        static_assert(std::is_base_of<Resource, T>::value, "The object must derive from Resource!");

        auto& typeResources = resources[std::type_index(typeid(T))];
        auto& typeRefCounts = refCounts[std::type_index(typeid(T))];
        auto it = typeResources.find(name);

        if (it != typeResources.end()) {
            typeRefCounts[name]++;
            return ResourceHandle<T>(name, this);
        }

        auto resource = std::make_shared<T>(name);
        if (!resource -> Load()) {
            //Failed to load
            return ResourceHandle<T>();
        }

        typeResources[name] = resource;
        typeRefCounts[name] = 1;

        return ResourceHandle<T>(name, this);
    }

    template<typename T>
    T* ResourceManager::GetResource(const std::string& name) {
        static_assert(std::is_base_of<Resource, T>::value, "The object must derive from Resource!");

        auto& typeResources = resources[std::type_index(typeid(T))];
        auto it = typeResources.find(name);

        if (it == typeResources.end()) {
            return nullptr;
        }

        return static_cast<T*>(it->second.get());
    }

    template<typename T>
    bool ResourceManager::HasResource(const std::string& name) {
        static_assert(std::is_base_of<Resource, T>::value, "The object must derive from Resource!");

        auto& typeResources = resources[std::type_index(typeid(T))];
        auto it = typeResources.find(name);

        if (it == typeResources.end()) {
            return false;
        }

        return true;
    }

    template<typename T>
    void ResourceManager::Release(const std::string& name) {
        static_assert(std::is_base_of<Resource, T>::value, "The object must derive from Resource!");

        auto& typeRefCounts = refCounts[std::type_index(typeid(T))];
        auto& typeResources = resources[std::type_index(typeid(T))];
        auto it = typeRefCounts.find(name);
        if (it != typeRefCounts.end()) {
            it -> second--;

            if (it -> second <= 0) {
                auto resourceIt = typeResources.find(name);
                if (resourceIt != typeResources.end()) {
                    resourceIt -> second -> Unload();
                    typeResources.erase(resourceIt);
                }
                
                typeRefCounts.erase(it);
            }

        }
    }

    void ResourceManager::UnloadAll() {
        for (auto& [type, typeResources]: resources) {
            for (auto& [name, resource] : typeResources) {
                resource -> Unload();
            }
            typeResources.clear();
        }
        refCounts.clear();
    }
}