#include "error.hpp"

namespace gl {
void ClearError() noexcept {
    while (glGetError() != GL_NO_ERROR) {
    }
}

std::string ErrorCat::message(int code) const {
    switch (code) {
        case GL_NO_ERROR:
            return "NO_ERROR";
        case GL_INVALID_ENUM:
            return "INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "INVALID_OPERATION";
        case GL_STACK_OVERFLOW:
            return "STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW:
            return "STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY:
            return "OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "INVALID_FRAMEBUFFER_OPERATION";
        case SHADER_FILE_NOT_FOUND:
            return "SHADER_FILE_NOT_FOUND";
        case SHADER_ERROR:
            return "SHADER_ERROR";
        case PROGRAM_ERROR:
            return "PROGRAM_ERROR";
        default:
            return "Unknown Opengl Error";
    }
}

const char *ErrorCat::name() const noexcept {
    return "OpenGLError";
}

const ErrorCat &ErrorCat::Get() {
    static ErrorCat instance;
    return instance;
}

Error MakeError(int64_t code, std::string extra) {
    return {{static_cast<int>(code), ErrorCat::Get()}, std::move(extra)};
}

} // namespace gl