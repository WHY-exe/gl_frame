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
    DEL_COPY_CTOR(Window);
    DEL_MOVE_CTOR(Window);

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