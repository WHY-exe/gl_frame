#include "texture.h"
#include "GL/glew.h"
#include "common/image_loader.h"
#include "error.h"
#include <vector>

namespace gl {
Texture2D::Texture2D(int num) noexcept : num_(num) {
    GLErrorInit;
    GLCall(glGenTextures(num, &handle_));
}

Texture2D::~Texture2D() noexcept {
    glDeleteTextures(num_, &handle_);
}

bool Texture2D::Bind() noexcept {
    GLErrorInit;
    GLCall(glBindTexture(GL_TEXTURE_2D, handle_));
    return GLErrorResult;
}

bool Texture2D::SetParam(const std::vector<Filter> &filters) noexcept {
    GLErrorInit;
    for (const auto &i : filters) {
        GLCall(glTexParameteri(GL_TEXTURE_2D, i.name, i.type));
        if (!GLErrorResult) {
            break;
        }
    }
    return GLErrorResult;
}

bool Texture2D::SetData(const util::ImageLoader &loader) noexcept {
    GLErrorInit;
    if (!loader.IsInit()) {
        return false;
    }
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, loader.GetWidth(), loader.GetHeight(), 0, GL_RGB,
                        GL_UNSIGNED_BYTE, loader.GetBuffer()));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    return GLErrorResult;
}

} // namespace gl