#include "vertex.h"

#include "GL/glew.h"
#include "error.h"

namespace gl {
namespace vertex {

    Buffer::Buffer(uint32_t count) noexcept {
        // TODO: exception handling
        Init(count);
    }

    Buffer::~Buffer() noexcept {
        if (IsInit()) {
            glDeleteBuffers(1, &handle_);
        }
    }

    bool Buffer::Init(uint32_t count) noexcept {
        // generate one buffer object and return a uint
        // to represent buffer
        GLErrorInit;
        GLCall(glGenBuffers(count, &handle_));
        return GLErrorResult;
    }

    void Buffer::SetBuffer(std::vector<float> &&vertex_data) noexcept {
        buffer_ = std::move(vertex_data);
    }

    bool Buffer::Bind() noexcept {
        GLErrorInit;
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, handle_));
        // copy the vertices data to the buffer object
        // with the specific type that currently bound
        // to the pipline
        if (!buffer_.empty()) {
            GLCall(glBufferData(GL_ARRAY_BUFFER, buffer_.size() * sizeof(float), buffer_.data(),
                                GL_STATIC_DRAW));
        }
        return GLErrorResult;
    }

    Layout::Layout(uint32_t count) noexcept {
        // TODO: error handling
        Init(count);
    }

    Layout::~Layout() noexcept {
        if (IsInit()) {
            glDeleteVertexArrays(1, &handle_);
        }
    }

    bool Layout::Init(uint32_t count) noexcept {
        GLErrorInit;
        GLCall(glGenVertexArrays(count, &handle_));
        return GLErrorResult;
    }

    bool Layout::Bind() noexcept {
        GLErrorInit;
        GLCall(glBindVertexArray(handle_));
        return GLErrorResult;
    }

    bool Layout::SetAttribute(const LayoutAttri &attri) noexcept {
        GLErrorInit;
        GLCall(glVertexAttribPointer(attri.index, attri.size, attri.type, attri.normalize,
                                     attri.stride, attri.offset));
        GLCall(glEnableVertexAttribArray(attri.index));
        return GLErrorResult;
    }

} // namespace vertex
} // namespace gl
