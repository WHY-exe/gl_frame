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
    glfwSetFramebufferSizeCallback(
        window_, [](GLFWwindow *, int width, int height) { glViewport(0, 0, width, height); });
    return IsInit();
}

bool Window::IsInit() const noexcept {
    return window_ != nullptr;
}

int Window::ShouldClose() noexcept {
    return glfwWindowShouldClose(window_);
}

void Window::SwapBuffer() noexcept {
    if (window_) {
        glfwSwapBuffers(window_);
    }
}

void Window::SetWindowCurrent() noexcept {
    if (window_) {
        glfwMakeContextCurrent(window_);
    }
};

} // namespace glfw
