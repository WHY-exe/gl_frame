#include "window.h"

#include "GLFW/glfw3.h"
namespace glfw {
Window::Window() noexcept : m_window(nullptr), m_framebuffersized_cb() {}

Window::Window(int width, int height, const std::string& title,
               GLFWmonitor* monitor, GLFWwindow* shared)
    : Window() {
  Init(width, height, title, monitor, shared);
}

bool Window::Init(int width, int height, const std::string& title,
                  GLFWmonitor* monitor, GLFWwindow* shared) noexcept {
  m_window = std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>>(
      glfwCreateWindow(width, height, title.c_str(), monitor, shared),
      [](GLFWwindow* pwindow) {
        if (pwindow) {
          glfwDestroyWindow(pwindow);
        }
      });
  glfwSetWindowUserPointer(m_window.get(), this);
  return IsInit();
}

bool Window::IsInit() const noexcept { return m_window != nullptr; }

void Window::OnFrameBufferSized(FrameBufferSizedCallback&& callback) noexcept {
  m_framebuffersized_cb = std::move(callback);
  glfwSetFramebufferSizeCallback(
      m_window.get(), [](GLFWwindow* window, int width, int height) {
        Window* pThis = (Window*)glfwGetWindowUserPointer(window);
        pThis->m_framebuffersized_cb(*pThis, width, height);
      });
}

void Window::SetWindowCurrent() noexcept {
  glfwMakeContextCurrent(m_window.get());
}

void Window::Update(RenderCallBack&& callback) {
  m_render_callback = std::move(callback);
}

bool Window::Run() {
  bool callback_ret = false;
  while (!glfwWindowShouldClose(m_window.get())) {
    callback_ret = m_render_callback(*this);
    if (!callback_ret) {
      break;
    }
    /* Swap front and back buffers */
    glfwSwapBuffers(m_window.get());

    /* Poll for and process events */
    glfwPollEvents();
  }
  return callback_ret;
}

}  // namespace glfw
