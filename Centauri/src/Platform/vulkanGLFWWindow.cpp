#include "vulkanGLFWWindow.h"
#include "../Events/windowEvent.h"

namespace Centauri {

    VulkanGLFWWindow::VulkanGLFWWindow(const SurfaceProps& props) {
        height = props.height;
        width = props.width;
        title = props.title;

        Initialize();
    }

    void VulkanGLFWWindow::OnUpdate() {
        glfwPollEvents();
    }

    void VulkanGLFWWindow::Initialize() {
        glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
	    glfwSetFramebufferSizeCallback(window, OnResize);
        

    }

    unsigned int VulkanGLFWWindow::GetWidth() {
        //With this we ensure the correct size
		glfwGetFramebufferSize(window, &width, &height);
        return width;
    }

    unsigned int VulkanGLFWWindow::GetHeight() {
		glfwGetFramebufferSize(window, &width, &height);
        return height;
    }

    VulkanGLFWWindow::~VulkanGLFWWindow() {
        glfwDestroyWindow(window);
    }

    Surface* Surface::CreateSurface(const SurfaceProps& props) {
        return new VulkanGLFWWindow(props);
    }

    void VulkanGLFWWindow::OnResize(GLFWwindow* window, int width, int height) {
        WindowResizedEvent windowResize(width, height);
        auto surf = reinterpret_cast<VulkanGLFWWindow*>(glfwGetWindowUserPointer(window));
        surf -> eventSystem -> PublishEvent(windowResize);

        surf -> width = width;
        surf -> height = height; 
    }

    bool VulkanGLFWWindow::Closed() const {
        return glfwWindowShouldClose(window);
    }

    VkSurfaceKHR VulkanGLFWWindow::GetVulkanSurface(const VkInstance& instance) const {
        VkSurfaceKHR _surf;
        if (window == nullptr) throw std::runtime_error("null window??");

		if (glfwCreateWindowSurface(instance, window, nullptr, &_surf) != 0) {
			throw std::runtime_error("failed to create window surface!");
		}
        return _surf;
    }

}