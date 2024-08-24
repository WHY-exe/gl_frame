#include "shader.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "GL/glew.h"
#include "error.h"
#include "program.h"
#include "spdlog/spdlog.h"
namespace gl {
Shader::Shader(Program &program) noexcept : program_(program), is_init_(false) {}

Shader::Shader(Program &program, ShaderType type, const std::string &shader_path)
    : Shader(program) {
    is_init_ = InitFromFile(type, shader_path);
}

bool Shader::InitFromFile(ShaderType type, const std::string &shader_path) {
    if (!std::filesystem::exists(shader_path)) {
        spdlog::error("shader path {} no exists", shader_path);
        return false;
    }
    const std::ifstream ifs(shader_path);
    std::stringstream   ss;
    ss << ifs.rdbuf();
    return InitFromSrc(type, ss.str());
}

bool Shader::InitFromSrc(ShaderType type, const std::string &shader_src) noexcept {
    handle_                = glCreateShader(static_cast<uint32_t>(type));
    const char *pcode      = shader_src.c_str();
    int         src_length = static_cast<int>(shader_src.length());
    glShaderSource(handle_, 1, &pcode, &src_length);
    glCompileShader(handle_);
    // check if compliation is successful
    int success, info_len;
    glGetShaderiv(handle_, GL_COMPILE_STATUS, &success);
    if (!success) {
        // get the info log length
        glGetShaderiv(handle_, GL_INFO_LOG_LENGTH, &info_len);
        std::unique_ptr<char[]> info_log = std::make_unique<char[]>(info_len);

        glGetShaderInfoLog(handle_, info_len, NULL, info_log.get());
        spdlog::error("SHADER::COMPILATION_FAILED: {}", info_log.get());
        return is_init_;
    }
    is_init_ = true;
    return is_init_;
}

Shader::~Shader() {
    Destroy();
}

bool Shader::Bind() noexcept {
    GLErrorInit;
    GLCall(glAttachShader(program_.GetID(), handle_));
    return GLErrorResult;
}

void Shader::Destroy() noexcept {
    if (!IsInit()) {
        glDeleteShader(handle_);
        handle_ = 0;
    }
}

bool Shader::IsInit() const noexcept {
    return is_init_;
}

} // namespace gl
