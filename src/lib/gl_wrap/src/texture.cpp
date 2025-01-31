#include "texture.h"
#include "GL/glew.h"
#include "common/image_loader.h"
#include <vector>

namespace gl {
Result<Texture2D> Texture2D::New(int num) noexcept {
    Texture2D texture;
    texture.num_ = num;
    RET_IF_ERROR(CheckError(glGenTextures, texture.num_, &texture.handle_));
    texture.is_init_ = true;
    return texture;
}

Texture2D::~Texture2D() noexcept {
    if (is_init_) {
        glDeleteTextures(num_, &handle_);
        is_init_ = false;
    }
}

Result<void> Texture2D::Bind() noexcept {
    RET_IF_ERROR(CheckError(glBindTexture, GL_TEXTURE_2D, handle_));
    return {};
}

Result<void> Texture2D::SetParam(const std::vector<Filter> &filters) noexcept {
    for (const auto &i : filters) {
        RET_IF_ERROR(CheckError(glTexParameteri, GL_TEXTURE_2D, i.name, i.type));
    }
    return {};
}

Result<void> Texture2D::SetData(const util::ImageLoader &loader) noexcept {
    RET_IF_ERROR(CheckError(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGB, loader.GetWidth(),
                            loader.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, loader.GetBuffer()));
    RET_IF_ERROR(CheckError(glGenerateMipmap, GL_TEXTURE_2D));
    return {};
}

} // namespace gl