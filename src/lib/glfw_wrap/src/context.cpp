#include "context.h"
#include "GLFW/glfw3.h"
#include <cpptrace/cpptrace.hpp>

namespace glfw {
Context::Context(int version_major, int version_minor) {
    if (glfwInit() == GLFW_FALSE) {
        throw cpptrace::runtime_error("fail to init glfw");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Context::~Context() noexcept {
    glfwTerminate();
}

} // namespace glfw