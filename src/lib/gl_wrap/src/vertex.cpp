#include "vertex.h"

#include "GL/glew.h"
#include "error.hpp"

namespace gl {
namespace vertex {
    Buffer::~Buffer() noexcept {
        if (is_init_) {
            glDeleteBuffers(count_, &handle_);
        }
    }

    Result<Buffer> Buffer::New(int count) noexcept {
        // generate one buffer object and return a uint
        // to represent buffer
        Buffer buffer;
        buffer.count_ = count;
        RET_IF_ERROR(CheckError(glGenBuffers, buffer.count_, &buffer.handle_));
        buffer.is_init_ = true;
        return buffer;
    }

    void Buffer::SetBuffer(std::vector<float> &&vertex_data) noexcept {
        buffer_ = std::move(vertex_data);
    }

    Result<void> Buffer::Bind() noexcept {
        RET_IF_ERROR(CheckError(glBindBuffer, GL_ARRAY_BUFFER, handle_));
        // copy the vertices data to the buffer object
        // with the specific type that currently bound
        // to the pipline
        if (!buffer_.empty()) {
            RET_IF_ERROR(CheckError(glBufferData, GL_ARRAY_BUFFER, buffer_.size() * sizeof(float),
                                    buffer_.data(), GL_STATIC_DRAW));
        }
        return {};
    }

    Layout::~Layout() noexcept {
        if (is_init_) {
            glDeleteVertexArrays(1, &handle_);
        }
    }

    Result<Layout> Layout::New(int count) noexcept {
        Layout layout;
        layout.count_ = count;
        RET_IF_ERROR(CheckError(glGenVertexArrays, count, &layout.handle_));
        layout.is_init_ = true;
        return layout;
    }

    Result<void> Layout::Bind() noexcept {
        RET_IF_ERROR(CheckError(glBindVertexArray, handle_));
        return {};
    }

    Result<void> Layout::SetAttribute(const LayoutAttri &attri) noexcept {
        RET_IF_ERROR(CheckError(glVertexAttribPointer, attri.index, attri.size, attri.type,
                                attri.normalize, attri.stride, attri.offset));
        RET_IF_ERROR(CheckError(glEnableVertexAttribArray, attri.index));
        return {};
    }

} // namespace vertex
} // namespace gl
