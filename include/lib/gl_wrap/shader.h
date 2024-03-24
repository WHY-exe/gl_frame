#pragma once
#include <cstdint>
#include <string>

namespace gl {

enum class ShaderType {
  UNKNOWN = 0x0000,
  VERTEX = 0x8B31,
  FRAGMENT = 0x8B30,
  GEOMETRY = 0x8DD9
};

class Shader {
 private:
  uint32_t m_shader;
  bool m_is_init;

 public:
  Shader() noexcept;
  Shader(ShaderType type, const std::string& shader_path);

  bool InitFromFile(ShaderType type, const std::string& shader_path);
  bool InitFromSrc(ShaderType type, const std::string& shader_src) noexcept;

  ~Shader();

  void Destroy() noexcept;
  bool IsInit() noexcept;
  uint32_t Get() noexcept;
};

}  // namespace gl
