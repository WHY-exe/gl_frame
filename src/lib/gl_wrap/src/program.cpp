#include "program.h"

#include "GL/glew.h"
#include "shader.h"
#include <spdlog/spdlog.h>


namespace gl {
Result<Program> Program::New() noexcept{
    Program program;
    auto program_handle = CheckError(glCreateProgram);
    RET_IF_ERROR(program_handle);
    program.handle_ = *program_handle;
    program.is_init_ = true;
    return program;
}

Program::~Program() noexcept {
    if (is_init_) {
        glDeleteProgram(handle_);
        is_init_ = false;
    }
}

Result<void> Program::AttachShader(Shader &shader) noexcept {
    return shader.Bind();
}

Result<void> Program::AttachShader(gl::ShaderType type, const std::string_view &shader_src) {
    auto shader = Shader::New(handle_, type, shader_src);
    RET_IF_ERROR(shader) ; 
    return AttachShader(*shader);
}

Result<void> Program::SetUniformValue(const std::string       &name,
                                                    std::variant<int, float> value) {
    const auto uniform_locaion = gl::CheckError(glGetUniformLocation, handle_, name.c_str());
    if (!uniform_locaion) {
        return tl::unexpected(uniform_locaion.error());
    }
    return std::visit(
        [uniform_locaion](auto &&arg) -> gl::Result<void> {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same<T, int>()) {
                RET_IF_ERROR(gl::CheckError(glUniform1i, *uniform_locaion, arg));
                return {};
            } else if constexpr (std::is_same<T, float>()) {
                RET_IF_ERROR(gl::CheckError(glUniform1f, *uniform_locaion, arg));
                return {};
            } else {
                assert("unknown data type");
            }
        },
        value);
}

Result<void> Program::Bind() noexcept {
    RET_IF_ERROR(gl::CheckError(glLinkProgram, handle_));
    int success, info_len;
    glGetProgramiv(handle_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &info_len);
        std::string info_log(info_len, '\0');
        glGetProgramInfoLog(handle_, 512, nullptr, &info_log[0]);
        return tl::unexpected(gl::MakeError(PROGRAM_ERROR, std::move(info_log)));
    }
    return {};
}

void Program::Use() noexcept {
    glUseProgram(handle_);
}

} // namespace gl
