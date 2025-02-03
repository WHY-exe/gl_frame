#pragma once
#include <GL/glew.h>

#include "glfw_wrap/context.h"
#include "gl_wrap/program.h"
#include "glfw_wrap/window.h"
#include "common/macro.h"

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
    DEL_COPY_CTOR(App);
    DEL_MOVE_CTOR(App);
};

} // namespace sandbox