#include "GL/glew.h"
#include "error.hpp"
#include "index.h"
namespace gl {

IndexBuffer::~IndexBuffer() noexcept {
    if (is_init_) {
        glDeleteBuffers(count_, &handle_);
        is_init_ = false;
    }
}

Result<IndexBuffer> IndexBuffer::New(int count) noexcept {
    IndexBuffer index_buffer;
    index_buffer.count_ = count;
    RET_IF_ERROR(gl::CheckError(glGenBuffers, count, &index_buffer.handle_));
    index_buffer.is_init_ = true;
    return index_buffer;
}

gl::Result<void> IndexBuffer::BindBuffer(std::vector<uint32_t> indicies) {
    buffer_ = std::move(indicies);
    return Bind();
}

size_t IndexBuffer::GetBufferSize() const noexcept {
    return buffer_.size();
}

Result<void> IndexBuffer::Bind() noexcept {
    RET_IF_ERROR(gl::CheckError(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, handle_));
    RET_IF_ERROR(gl::CheckError(glBufferData, GL_ELEMENT_ARRAY_BUFFER,
                                buffer_.size() * sizeof(uint32_t), buffer_.data(), GL_STATIC_DRAW));
    return {};
}

} // namespace gl