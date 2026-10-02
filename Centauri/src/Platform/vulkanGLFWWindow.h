#ifndef VULKAN_WINDOW_GLFW_H
#define VULKAN_WINDOW_GLFW_H

#include "vulkanSurface.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



namespace Centauri {
    class VulkanGLFWWindow : public VulkanSurface {
      public:
        VulkanGLFWWindow(const SurfaceProps& props);
        virtual ~VulkanGLFWWindow() override;
        virtual void OnUpdate() override;

        virtual void Initialize() override;
        virtual unsigned int GetWidth() const override;
        virtual unsigned int GetHeight() const override;
        virtual bool Closed() const override;

        virtual VkSurfaceKHR GetVulkanSurface(VkInstance& instance) override;


        //static Surface* Create(const SurfaceProps& props) override;
      private:
        GLFWwindow *window = nullptr;
        
        static void OnResize(GLFWwindow* window, int width, int height);
    };
}

#endif //VULKAN_WINDOW_GLFW_H