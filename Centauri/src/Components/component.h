#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

namespace Centauri {
    class Component {
      public:
        virtual ~Component() = default;

        virtual void Initialize() {}
        virtual void Update(float deltaTime) {}
        virtual void Render() {}

        void SetOwner(Entity* entity) {owner = entity;}
        Entity* GetOwner() const {return owner;}

      protected:
        Entity* owner;
    };
}

#endif //COMPONENT_H