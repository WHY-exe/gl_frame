#pragma once
#include <cstdint>
#include <string>

#include "bindable.h"
namespace gl {

enum class ShaderType {
  UNKNOWN = 0x0000,
  VERTEX = 0x8B31,
  FRAGMENT = 0x8B30,
  GEOMETRY = 0x8DD9
};
class Program;
class Shader : public Bindable {
  friend class Program;

 private:
  Program& program_;
  bool is_init_;

 public:
  Shader(Program& program) noexcept;
  ~Shader() noexcept;

  Shader(Program& program, ShaderType type, const std::string& shader_path);

  bool InitFromFile(ShaderType type, const std::string& shader_path);
  bool InitFromSrc(ShaderType type, const std::string& shader_src) noexcept;

  bool Bind() noexcept override final;
  void Destroy() noexcept;

  bool IsInit() const noexcept override;
};

}  // namespace gl
