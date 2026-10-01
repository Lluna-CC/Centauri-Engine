#include "windowGLFW.h"

namespace Centauri {

    WindowGLFW::WindowGLFW(const SurfaceProps& props) {
        height = props.height;
        width = props.width;
        title = props.title;
    }

    void WindowGLFW::OnUpdate() {
        
    }

    void WindowGLFW::Initialize() {
        glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
	    glfwSetFramebufferSizeCallback(window, OnResize);
    }

    unsigned int WindowGLFW::GetWidth() const {
        //We have to ensure that we keep track of width and height during the resize events and the initial creation
        return width;
    }

    unsigned int WindowGLFW::GetHeight() const {
        return height;
    }

    WindowGLFW::~WindowGLFW() {
        glfwDestroyWindow(window);
    }

    Surface* Surface::Create(const SurfaceProps& props) {
        return new WindowGLFW(props);
    }

    void WindowGLFW::OnResize(GLFWwindow* window, int width, int height) {
        
    }
}