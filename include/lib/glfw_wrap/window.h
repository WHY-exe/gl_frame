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
  using RenderCallBack = std::function<bool(Window&)>;
  using FrameBufferSizedCallback = std::function<void(Window&, int, int)>;

 private:
  WindowContext m_window;
  RenderCallBack m_render_callback;
  FrameBufferSizedCallback m_framebuffersized_cb;

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
  bool IsInit() const noexcept;

  void OnFrameBufferSized(FrameBufferSizedCallback&& callback) noexcept;
  void SetWindowCurrent() noexcept;
  // function called per-frame
  void Update(RenderCallBack&& callback);

  bool Run();
};

}  // namespace glfw