#pragma once
#include <string>
#include <variant>

#include "bindable.h"
namespace gl {
enum class ShaderType;
class Shader;

class Program : public Bindable {
    friend class Shader;

public:
    Program() noexcept;
    ~Program() noexcept;
    bool AttachShader(Shader &shader) noexcept;
    bool AttachShader(gl::ShaderType type, const std::string &path);

    bool Bind() noexcept final;
    void Use() noexcept;

    void SetUniformValue(const std::string &name, std::variant<int, float> value);
};

} // namespace gl
