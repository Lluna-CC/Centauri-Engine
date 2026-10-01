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

        bool Load() {
            loaded = doLoad();
            return loaded;
        }

        void Unload() {
            doUnload();
            loaded = false;
        }

      protected:
        virtual bool doLoad() = 0;
        virtual bool doUnload() = 0;  

      private:
        std::string name;
        bool loaded = false;
    };
    
    template<typename T>
    class ResourceHandle;
    
    class ResourceManager {
      public:
        template<typename T>
        ResourceHandle<T> Load(const std::string& name);

        template<typename T>
        T* GetResource(const std::string& name);

        template<typename T>
        bool HasResource(const std::string& name);

        template<typename T>
        void Release(const std::string& name);

        void UnloadAll();

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