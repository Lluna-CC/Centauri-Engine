#ifndef APPLICATION_H
#define APPLICATION_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Centauri {
  class Application {
    public:
      Application() = default;
      virtual ~Application() = default;

      void init();

      void run();

      //To be defined externally
      Application* CreateApplication();

    private:
      GLFWwindow *window = nullptr;
  };
}
#endif //APPLICATION_H