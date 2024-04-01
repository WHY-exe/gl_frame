#include "GL/glew.h"
#include "error.h"
#include "index.h"
namespace gl {
IndexBuffer::IndexBuffer(uint32_t count) noexcept : Bindable() {
  // TODO: error handling
  Init(count);
}

IndexBuffer::~IndexBuffer() noexcept {
  if (IsInit()) {
    glDeleteBuffers(1, &identifier_);
  }
}

bool IndexBuffer::Init(uint32_t count) {
  GLErrorInit;
  GLCall(glGenBuffers(count, &identifier_));
  return GLErrorResult;
}

void IndexBuffer::SetBuffer(std::vector<uint32_t>&& indicies) {
  buffer_ = std::move(indicies);
}

size_t IndexBuffer::GetBufferSize() const noexcept { return buffer_.size(); }

bool IndexBuffer::Bind() noexcept {
  GLErrorInit;
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, identifier_));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      buffer_.size() * sizeof(uint32_t), buffer_.data(),
                      GL_STATIC_DRAW));
  return GLErrorResult;
}

}  // namespace gl