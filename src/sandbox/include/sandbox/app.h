#pragma once
#include "GL/glew.h"
#include "glfw_wrap/context.h"
#include "gl_wrap/program.h"
#include "glfw_wrap/window.h"

namespace sandbox {
class App {
private:
    glfw::Context context_;
    glfw::Window  window_;

public:
    App();
    ~App() noexcept = default;
    void DoRender(gl::Program &program) noexcept;
    void DoLogic() noexcept;
    int  Run();

private:
    bool InitWindow() noexcept;
};

} // namespace sandbox