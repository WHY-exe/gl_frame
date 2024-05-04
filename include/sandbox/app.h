#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "gl_wrap/program.h"
#include "glfw_wrap/window.h"

namespace sandbox {
class App {
private:
  glfw::Window window_;

public:
  App() noexcept = default;
  inline ~App() noexcept { glfwTerminate(); };
  bool InitGLFW() noexcept;
  bool InitWindow() noexcept;
  void DoRender(gl::Program &program) noexcept;
  void DoLogic() noexcept;
  inline uint32_t InitGLEW() noexcept { return glewInit(); };
  int Run();
};

} // namespace sandbox