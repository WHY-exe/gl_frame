#pragma once
#include "keyboard.h"
#include <GLFW/glfw3.h>
#include <functional>
#include <string>

namespace glfw {
using FrameBufferSizedCallback = std::function<void(int, int)>;

class Window {
private:
    GLFWwindow *window_ = nullptr;

public:
    Keyboard kbd;

public:
    Window(const Window &)            = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&)                 = delete;
    Window &operator=(Window &&)      = delete;

    Window() noexcept = default;
    ~Window() noexcept;
    Window(int width, int height, const std::string &title, GLFWmonitor *monitor = nullptr,
           GLFWwindow *shared = nullptr);

    bool Init(int width, int height, const std::string &title, GLFWmonitor *monitor = nullptr,
              GLFWwindow *shared = nullptr) noexcept;

    bool IsInit() const noexcept;
    // inidicate that a glfw should close
    int  ShouldClose() noexcept;
    void SwapBuffer() noexcept;
    void SetWindowCurrent() noexcept;

    void BindCallback();
};

inline void PollEvents() {
    glfwPollEvents();
};
} // namespace glfw