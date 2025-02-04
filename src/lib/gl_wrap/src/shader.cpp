#include "shader.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include "error.hpp"
#include "program.h"
#include <spdlog/spdlog.h>

namespace gl {
Result<Shader> Shader::New(uint32_t program, ShaderType type, const std::string_view &shader_src) {
    Shader shader;
    shader.program_ = program;
    shader.type_    = type;
    RET_IF_ERROR(shader.InitFromSrc(shader_src));
    return shader;
}

Result<Shader> Shader::New(uint32_t program, const std::filesystem::path &shader_path) {
    Shader shader;
    shader.program_ = program;
    if (shader_path.extension() == "vs") {
        shader.type_ = ShaderType::VERTEX;
    } else if (shader_path.extension() == "fs") {
        shader.type_ = ShaderType::FRAGMENT;
    } else if (shader_path.extension() == "gs") {
        shader.type_ = ShaderType::GEOMETRY;
    } else {
        shader.type_ = ShaderType::UNKNOWN;
    }
    RET_IF_ERROR(shader.InitFromFile(shader_path));
    return shader;
}

Result<void> Shader::InitFromFile(const std::filesystem::path &shader_path) {
    if (!std::filesystem::exists(shader_path)) {
        return tl::unexpected(MakeError(SHADER_FILE_NOT_FOUND, "Shader file not found in path"));
    }
    const std::ifstream ifs(shader_path);
    std::stringstream   ss;
    ss << ifs.rdbuf();
    return InitFromSrc(ss.str());
}

Result<void> Shader::InitFromSrc(const std::string_view &shader_src) noexcept {
    auto shader_res = CheckError(glCreateShader, static_cast<uint32_t>(type_));
    if (!shader_res) {
        return tl::unexpected(shader_res.error());
    }
    handle_                = *shader_res;
    is_init_               = true;
    const char *pcode      = shader_src.data();
    int         src_length = static_cast<int>(shader_src.length());
    RET_IF_ERROR(CheckError(glShaderSource, handle_, 1, &pcode, &src_length));
    RET_IF_ERROR(CheckError(glCompileShader, handle_));
    // check if compliation is successful
    int success = 0, info_len = 0;
    glGetShaderiv(handle_, GL_COMPILE_STATUS, &success);
    if (!success) {
        // get the info log length
        glGetShaderiv(handle_, GL_INFO_LOG_LENGTH, &info_len);
        std::string info_log(info_len, '\0');
        glGetShaderInfoLog(handle_, info_len, nullptr, &info_log[0]);
        info_log.resize(info_log.find_first_of('\n'));
        return tl::unexpected(gl::MakeError(SHADER_ERROR, std::move(info_log)));
    }
    return {};
}

Shader::~Shader() noexcept {
    Destroy();
}

Result<void> Shader::Bind() noexcept {
    RET_IF_ERROR(CheckError(glAttachShader, program_, handle_));
    return {};
}

void Shader::Destroy() noexcept {
    if (is_init_) {
        glDeleteShader(handle_);
        is_init_ = false;
    }
}

} // namespace gl
