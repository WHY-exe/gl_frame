#pragma once
#include <cstdint>
#include <string>
#include <variant>

namespace gl {
enum class ShaderType;
class Shader;

class Program {
 private:
  uint32_t m_program;

 public:
  Program() noexcept;
  ~Program() noexcept;
  bool IsInit() const noexcept;
  void AttachShader(Shader& shader) noexcept;
  void AttachShader(gl::ShaderType type, const std::string& path);

  bool Link() noexcept;

  void SetUniformValue(const std::string& name, std::variant<int, float> value);

  void Install() noexcept;
  uint32_t Get() const noexcept;
};

}  // namespace gl
