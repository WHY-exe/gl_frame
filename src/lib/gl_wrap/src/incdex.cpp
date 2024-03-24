#include "GL/glew.h"
#include "error.h"
#include "index.h"
namespace gl {
namespace index {
Buffer::Buffer(uint32_t count) noexcept : m_ebo(0) {
  // TODO: error handling
  Init(count);
}

Buffer::~Buffer() noexcept {
  if (IsInit()) {
    glDeleteBuffers(1, &m_ebo);
  }
}

void Buffer::Init(uint32_t count) { GLCall(glGenBuffers(count, &m_ebo)); }

bool Buffer::IsInit() const noexcept { return m_ebo != 0; }

void Buffer::Bind(const std::vector<uint32_t>& indicies) noexcept {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                      indicies.size() * sizeof(uint32_t), indicies.data(),
                      GL_STATIC_DRAW));
}

}  // namespace index
}  // namespace gl