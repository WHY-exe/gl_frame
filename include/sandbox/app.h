#pragma once
#include <glfw_wrap/window.h>

#include <functional>
#include <memory>

namespace prj_exec1 {
class App {
 private:
  glfw::Window window_;

 private:
  bool InitGLFW() noexcept;
  uint32_t InitGLEW() noexcept;

 public:
  App();
  ~App() noexcept;
  void Run();
};

}  // namespace prj_exec1