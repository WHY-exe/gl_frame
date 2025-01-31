#pragma once
#include <cstdint>
#include <string>

#include "bindable.h"
#include "common/macro.h"
namespace gl {

enum class ShaderType { UNKNOWN = 0x0000, VERTEX = 0x8B31, FRAGMENT = 0x8B30, GEOMETRY = 0x8DD9 };
class Shader : public Bindable {

private:
    uint32_t   program_;
    ShaderType type_;

public:
    DEFAULT_MOVE_CTOR(Shader);
    ~Shader() noexcept override;

    static Result<Shader> New(uint32_t program, ShaderType type, const std::string &shader_path);

    gl::Result<void> Bind() noexcept final;
    void             Destroy() noexcept;

private:
    Result<void> InitFromFile(const std::string &shader_path);
    Result<void> InitFromSrc(const std::string &shader_src) noexcept;

private:
    Shader() noexcept = default;
    DEL_COPY_CTOR(Shader);
};

} // namespace gl
