#include "program.h"

#include "GL/glew.h"
#include "error.h"
#include "shader.h"
#include "spdlog/spdlog.h"

namespace gl {
Program::Program() noexcept : m_program(glCreateProgram()) {}

Program::~Program() noexcept {
  if (IsInit()) {
    glDeleteProgram(m_program);
  }
}

bool Program::IsInit() const noexcept { return m_program != 0; }

void Program::AttachShader(Shader& shader) noexcept {
  glAttachShader(m_program, shader.Get());
}

void Program::AttachShader(gl::ShaderType type, const std::string& path) {
  gl::Shader shader(type, path);
  if (!shader.IsInit()) {
    spdlog::error("Fail to init shader");
  } else {
    AttachShader(shader);
  }
}

uint32_t Program::Get() const noexcept { return m_program; }

void Program::SetUniformValue(const std::string& name,
                              std::variant<int, float> value) {
  const float uniform_locaion = glGetUniformLocation(m_program, name.c_str());
  if (uniform_locaion == -1) {
    spdlog::error("fail to fetch uniform location with name {}", name);
    return;
  }
  std::visit(
      [uniform_locaion](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
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

bool Program::Link() noexcept {
  if (!IsInit()) {
    spdlog::error("program is not init");
    return false;
  }

  glLinkProgram(m_program);
  int success, info_len;
  glGetProgramiv(m_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &info_len);
    std::unique_ptr<char> info_log = std::make_unique<char>(info_len);
    glGetProgramInfoLog(m_program, 512, nullptr, info_log.get());
    spdlog::error("fail to link shader program: {}", info_log.get());
    return false;
  }
  return true;
}

void Program::Install() noexcept { glUseProgram(m_program); }

}  // namespace gl
