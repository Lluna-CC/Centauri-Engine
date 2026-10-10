#ifndef APPLICATION_H
#define APPLICATION_H

#include "../Events/eventSystem.h"
#include "scene.h"
#include "../Platform/vulkanGLFWWindow.h"
#include "../Platform/vulkanPlatform.h"
#include "../Render/vulkanRenderer.h"

namespace Centauri {
  class Application: public EventListener {
    public:
      Application() = default;
      virtual ~Application();

      void Init();
      void Run();
      virtual void OnEvent(const Event& e);

    private:
      Surface* surface = nullptr;
      RenderPlatform* platform = nullptr;
      Renderer* renderer = nullptr;
  };
}
#endif //APPLICATION_H