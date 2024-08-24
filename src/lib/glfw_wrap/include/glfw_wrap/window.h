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
    Keyboard                 kbd;
    FrameBufferSizedCallback frameBufferSizedCallback;

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

    inline bool IsInit() const noexcept {
        return window_ != nullptr;
    };
    // inidicate that a glfw should close
    inline int ShouldClose() noexcept {
        return glfwWindowShouldClose(window_);
    }
    inline void SwapBuffer() noexcept {
        glfwSwapBuffers(window_);
    }
    inline void SetWindowCurrent() noexcept {
        glfwMakeContextCurrent(window_);
    };

    void BindCallback();
};

inline void PollEvents() {
    glfwPollEvents();
};
} // namespace glfw