#include "window.h"

#include "GLFW/glfw3.h"
namespace glfw {

void WndKeyCallback(GLFWwindow *window, int key, int, int action, int) {
    Window *pthis = static_cast<Window *>(glfwGetWindowUserPointer(window));
    pthis->kbd.OnKey(key, action);
}

void WndCharCallback(GLFWwindow *window, unsigned int codepoint) {
    Window *pthis = static_cast<Window *>(glfwGetWindowUserPointer(window));
    pthis->kbd.OnChar(static_cast<char>(codepoint));
}

Window::Window(int width, int height, const std::string &title, GLFWmonitor *monitor,
               GLFWwindow *shared) {
    Init(width, height, title, monitor, shared);
}

Window::~Window() noexcept {
    if (window_ != nullptr) {
        // glfwMakeContextCurrent(nullptr);
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
}

bool Window::Init(int width, int height, const std::string &title, GLFWmonitor *monitor,
                  GLFWwindow *shared) noexcept {
    window_ = glfwCreateWindow(width, height, title.c_str(), monitor, shared);
    glfwSetWindowUserPointer(window_, this);
    glfwSetKeyCallback(window_, WndKeyCallback);
    glfwSetCharCallback(window_, WndCharCallback);
    return IsInit();
}

void Window::BindCallback() {
    if (frameBufferSizedCallback) {
        glfwSetFramebufferSizeCallback(window_, [](GLFWwindow *window, int width, int height) {
            Window *this_ptr = static_cast<Window *>(glfwGetWindowUserPointer(window));
            this_ptr->frameBufferSizedCallback(width, height);
        });
    }
}

} // namespace glfw
