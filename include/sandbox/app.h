#pragma once
#include <glfw_wrap/window.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace sandbox {
class App {
private:
  glfw::Window window_;

public:
  App() noexcept = default;
  inline ~App() noexcept { glfwTerminate(); };
  bool InitGLFW() noexcept;
  bool InitWindow() noexcept;
  inline uint32_t InitGLEW() noexcept { return glewInit(); };
  int Run();
};

} // namespace sandbox