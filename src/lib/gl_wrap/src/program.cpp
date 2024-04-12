#include "program.h"

#include "GL/glew.h"
#include "error.h"
#include "shader.h"
#include "spdlog/spdlog.h"

namespace gl {
Program::Program() noexcept : Bindable() { identifier_ = glCreateProgram(); }

Program::~Program() noexcept {
  if (IsInit()) {
    glDeleteProgram(identifier_);
  }
}

bool Program::AttachShader(Shader &shader) noexcept { return shader.Bind(); }

bool Program::AttachShader(gl::ShaderType type, const std::string &path) {
  gl::Shader shader(*this, type, path);
  if (!shader.IsInit()) {
    spdlog::error("Fail to init shader");
    return false;
  } else {
    return AttachShader(shader);
  }
}

void Program::SetUniformValue(const std::string &name,
                              std::variant<int, float> value) {
  const int uniform_locaion = glGetUniformLocation(identifier_, name.c_str());
  if (uniform_locaion == -1) {
    spdlog::error("fail to fetch uniform location with name {}", name);
    return;
  }
  std::visit(
      [uniform_locaion](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        GLErrorInit;
        if constexpr (std::is_same<T, int>()) {
          GLCall(glUniform1i(uniform_locaion, arg));
        } else if constexpr (std::is_same<T, float>()) {
          GLCall(glUniform1f(uniform_locaion, arg));
        } else {
          spdlog::warn("unknown data type");
        }
      },
      value);
}

bool Program::Bind() noexcept {
  if (!IsInit()) {
    spdlog::error("program is not init");
    return false;
  }

  glLinkProgram(identifier_);
  int success, info_len;
  glGetProgramiv(identifier_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramiv(identifier_, GL_INFO_LOG_LENGTH, &info_len);
    std::unique_ptr<char[]> info_log = std::make_unique<char[]>(info_len);
    glGetProgramInfoLog(identifier_, 512, nullptr, info_log.get());
    spdlog::error("fail to link shader program: {}", info_log.get());
    return false;
  }
  return true;
}

void Program::Use() noexcept { glUseProgram(identifier_); }

} // namespace gl
