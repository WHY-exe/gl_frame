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
    DEFAULT_MOVE_CTOR(Program);
    static Result<Program> New() noexcept;

    ~Program() noexcept override;
    Result<void> AttachShader(Shader &shader) noexcept;
    Result<void> AttachShader(ShaderType type, const std::string &path);

    Result<void> Bind() noexcept final;
    Result<void> SetUniformValue(const std::string &name, std::variant<int, float> value);
    void         Use() noexcept;

private:
    Program() noexcept = default;
    DEL_COPY_CTOR(Program);
};

} // namespace gl
