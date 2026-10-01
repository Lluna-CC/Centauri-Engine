#ifndef WINDOW_GLFW_H
#define WINDOW_GLFW_H

#include "surface.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Centauri {
    class WindowGLFW : public Surface {
      public:
        WindowGLFW(const SurfaceProps& props);
        virtual ~WindowGLFW() override;
        virtual void OnUpdate() override;

        virtual void Initialize() override;
        virtual unsigned int GetWidth() const override;
        virtual unsigned int GetHeight() const override;

        //static Surface* Create(const SurfaceProps& props) override;
      private:
        GLFWwindow *window = nullptr;

        static void OnResize(GLFWwindow* window, int width, int height);
    };
}

#endif //WINDOW_GLFW_H