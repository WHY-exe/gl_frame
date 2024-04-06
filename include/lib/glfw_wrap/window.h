#pragma once
#include <functional>
#include <memory>
#include <string>
struct GLFWwindow;
struct GLFWmonitor;

namespace glfw {
class Window {
  using WindowContext =
      std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)> >;
  using RenderCallBack = std::function<bool(void)>;
  using FrameBufferSizedCallback = std::function<void(int, int)>;

 private:
  WindowContext window_;

 public:
  RenderCallBack renderCallback;
  FrameBufferSizedCallback frameBufferSizedCallback;

 public:
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window&&) = delete;
  ~Window() = default;

  Window() noexcept;
  Window(int width, int height, const std::string& title,
         GLFWmonitor* monitor = nullptr, GLFWwindow* shared = nullptr);

  bool Init(int width, int height, const std::string& title,
            GLFWmonitor* monitor = nullptr,
            GLFWwindow* shared = nullptr) noexcept;

  inline bool IsInit() const noexcept { return window_ != nullptr; };
  void SetWindowCurrent() noexcept;

  bool Start();
};

}  // namespace glfw