#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>

namespace Centauri {

    class Resource {
      public:
        explicit Resource(const std::string& id) : name(id) {}
        virtual ~Resource() = default;
        
        const std::string& GetName() const {return name;}
        bool IsLoaded() {return loaded;}

        virtual bool Load() = 0;
        virtual void Unload() = 0;

      private:
        std::string name;
        bool loaded = false;
    };
    
    template<typename T>
    class ResourceHandle;
    
    class ResourceManager {
      public:
          template<typename T, typename... Args>
          ResourceHandle<T> Load(const std::string& name, Args&&... args) {
          static_assert(std::is_base_of<Resource, T>::value, "The object must derive from Resource!");

          auto& typeResources = resources[std::type_index(typeid(T))];
          auto& typeRefCounts = refCounts[std::type_index(typeid(T))];
          auto it = typeResources.find(name);

          if (it != typeResources.end()) {
              typeRefCounts[name]++;
              return ResourceHandle<T>(name, this);
          }

          auto resource = std::make_shared<T>(name, std::forward<Args>(args)...);
          if (!resource -> Load()) {
              //Failed to load
              return ResourceHandle<T>();
          }

          typeResources[name] = resource;
          typeRefCounts[name] = 1;

          return ResourceHandle<T>(name, this);
        }

        template<typename T>
        T* GetResource(const std::string& name) {
          static_assert(std::is_base_of<Resource, T>::value, "The object must derive from Resource!");

          auto& typeResources = resources[std::type_index(typeid(T))];
          auto it = typeResources.find(name);

          if (it == typeResources.end()) {
              return nullptr;
          }

          return static_cast<T*>(it->second.get());
        } 

        template<typename T>
        bool HasResource(const std::string& name) {
          static_assert(std::is_base_of<Resource, T>::value, "The object must derive from Resource!");

          auto& typeResources = resources[std::type_index(typeid(T))];
          auto it = typeResources.find(name);

          if (it == typeResources.end()) {
              return false;
          }

          return true;
        }

        template<typename T>
        void Release(const std::string& name) {
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

        void UnloadAll() {
          for (auto& [type, typeResources]: resources) {
              for (auto& [name, resource] : typeResources) {
                  resource -> Unload();
              }
              typeResources.clear();
          }
          refCounts.clear();
        }

      private:
        std::unordered_map<std::type_index, std::unordered_map<std::string, std::shared_ptr<Resource>>> resources;

        std::unordered_map<std::type_index, std::unordered_map<std::string, int>> refCounts;
    };

    template<typename T>
    class ResourceHandle {
      public:
        ResourceHandle() : resourceManager(nullptr) {}
        ResourceHandle(const std::string& id, ResourceManager* manager) : name(id), resourceManager(manager) {}

        T* Get() const {
            if (!resourceManager) return nullptr;
            return resourceManager -> GetResource<T>(name);
        }

        bool IsValid() const {
            return resourceManager && resourceManager -> HasResource<T>(name);
        }

        const std::string& GetName() const {
            return name;
        }

        T* operator->() const {
            return Get();
        }

        T& operator*() const {
            return *Get();
        }

        operator bool() const {
            return IsValid();
        }


      private:
        std::string name;
        ResourceManager* resourceManager;
    };

}

#endif //RESOURCE_H