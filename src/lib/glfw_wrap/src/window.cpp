#include "window.h"

#include "GLFW/glfw3.h"
#include "common/log.h"
namespace glfw {
Window::Window() noexcept : window_(nullptr), frameBufferSizedCallback() {}

Window::Window(int width, int height, const std::string &title,
               GLFWmonitor *monitor, GLFWwindow *shared)
    : Window() {
  Init(width, height, title, monitor, shared);
}

bool Window::Init(int width, int height, const std::string &title,
                  GLFWmonitor *monitor, GLFWwindow *shared) noexcept {
  window_ = std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow *)>>(
      glfwCreateWindow(width, height, title.c_str(), monitor, shared),
      [](GLFWwindow *pwindow) {
        if (pwindow) {
          glfwDestroyWindow(pwindow);
        }
      });
  glfwSetWindowUserPointer(window_.get(), this);
  return IsInit();
}

void Window::SetWindowCurrent() noexcept {
  glfwMakeContextCurrent(window_.get());
};

bool Window::Start() {
  if (frameBufferSizedCallback) {
    glfwSetFramebufferSizeCallback(
        window_.get(), [](GLFWwindow *window, int width, int height) {
          Window *pThis = (Window *)glfwGetWindowUserPointer(window);
          pThis->frameBufferSizedCallback(width, height);
        });
  }

  bool callback_ret = false;
  while (!glfwWindowShouldClose(window_.get())) {
    if (!renderCallback) {
      LOG_ERROR_COUT("rendering logic is not set");
      break;
    }
    callback_ret = renderCallback();
    if (!callback_ret) {
      break;
    }
    /* Swap front and back buffers */
    glfwSwapBuffers(window_.get());

    /* Poll for and process events */
    glfwPollEvents();
  }
  return callback_ret;
}

} // namespace glfw
