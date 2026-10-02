#ifndef APPLICATION_H
#define APPLICATION_H

#include "../Events/eventSystem.h"
#include "scene.h"
#include "../Platform/vulkanGLFWWindow.h"

namespace Centauri {
  class Application: public EventListener {
    public:
      Application() = default;
      virtual ~Application() = default;

      void Init();
      void Run();
      virtual void OnEvent(const Event& e);

    private:
      Surface *surface = nullptr;
  };
}
#endif //APPLICATION_H