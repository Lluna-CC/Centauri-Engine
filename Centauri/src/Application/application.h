#ifndef APPLICATION_H
#define APPLICATION_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../Events/eventSystem.h"
#include "../Components/entity.h"

namespace Centauri {
  class Application: public EventListener {
    public:
      Application() = default;
      virtual ~Application() = default;

      void Init();
      void Run();
      virtual void OnEvent(const Event& e);

    private:
      //GLFWwindow *window = nullptr;
  };
}
#endif //APPLICATION_H