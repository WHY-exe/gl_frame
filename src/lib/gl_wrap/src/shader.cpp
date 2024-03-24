#include "shader.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "GL/glew.h"
#include "common/exception.h"
#include "spdlog/spdlog.h"

namespace gl {
Shader::Shader() noexcept : m_shader(0), m_is_init(false) {}

Shader::Shader(ShaderType type, const std::string& shader_path) {
  m_is_init = InitFromFile(type, shader_path);
}

bool Shader::InitFromFile(ShaderType type, const std::string& shader_path) {
  if (!std::filesystem::exists(shader_path)) {
    spdlog::error("shader path {} no exists", shader_path);
    return false;
  }
  const std::ifstream ifs(shader_path);
  std::stringstream ss;
  ss << ifs.rdbuf();
  return InitFromSrc(type, ss.str());
}

bool Shader::InitFromSrc(ShaderType type,
                         const std::string& shader_src) noexcept {
  m_shader = glCreateShader((uint32_t)type);
  const char* pcode = shader_src.c_str();
  int src_length = (int)shader_src.length();
  glShaderSource(m_shader, 1, &pcode, &src_length);
  glCompileShader(m_shader);
  // check if compliation is successful
  int success, info_len;
  glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    // get the info log length
    glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &info_len);
    std::unique_ptr<char[]> info_log = std::make_unique<char[]>(info_len);

    glGetShaderInfoLog(m_shader, info_len, NULL, info_log.get());
    spdlog::error("SHADER::COMPILATION_FAILED: {}", info_log.get());
    return m_is_init;
  }
  m_is_init = true;
  return m_is_init;
}

Shader::~Shader() { Destroy(); }

void Shader::Destroy() noexcept {
  if (!IsInit()) {
    glDeleteShader(m_shader);
    m_shader = 0;
  }
}

bool Shader::IsInit() noexcept { return m_is_init; }

uint32_t Shader::Get() noexcept { return m_shader; }

}  // namespace gl
