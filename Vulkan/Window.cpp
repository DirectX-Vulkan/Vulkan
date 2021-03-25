#include "Window.h"


Window::Window(int width, int height, string name)
{
    string windowname = "Vulkan - " + name;

    // initialize GLFW library
    glfwInit();
    // tell GLFW not to create OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // temporarily disable window resizing to prevent errors
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // create the window
    m_window = glfwCreateWindow(width, height, windowname.c_str(), nullptr, nullptr);

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, FramebufferResizeCallback);
}

void Window::CleanUp()
{
    // destroy the GLFW window
    glfwDestroyWindow(m_window);
    // terminate GLFW
    glfwTerminate();
}

GLFWwindow* Window::GetWindow()
{
    return m_window;
}

void Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto app = reinterpret_cast<Graphics*>(glfwGetWindowUserPointer(window));
    app->m_framebufferResized = true;
}
