#include "vulkanGLFWWindow.h"

namespace Centauri {

    VulkanGLFWWindow::VulkanGLFWWindow(const SurfaceProps& props) {
        height = props.height;
        width = props.width;
        title = props.title;
    }

    void VulkanGLFWWindow::OnUpdate() {
        
    }

    void VulkanGLFWWindow::Initialize() {
        glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
	    glfwSetFramebufferSizeCallback(window, OnResize);

    }

    unsigned int VulkanGLFWWindow::GetWidth() const {
        //We have to ensure that we keep track of width and height during the resize events and the initial creation
        return width;
    }

    unsigned int VulkanGLFWWindow::GetHeight() const {
        return height;
    }

    VulkanGLFWWindow::~VulkanGLFWWindow() {
        glfwDestroyWindow(window);
    }

    Surface* Surface::Create(const SurfaceProps& props) {
        return new VulkanGLFWWindow(props);
    }

    void VulkanGLFWWindow::OnResize(GLFWwindow* window, int width, int height) {
        
    }

    bool VulkanGLFWWindow::Closed() const {
        return glfwWindowShouldClose(window);
    }

    VkSurfaceKHR VulkanGLFWWindow::GetVulkanSurface(VkInstance& instance) {
        VkSurfaceKHR _surf;
		if (glfwCreateWindowSurface(instance, window, nullptr, &_surf) != 0) {
			throw std::runtime_error("failed to create window surface!");
		}
        return _surf;
    }

}