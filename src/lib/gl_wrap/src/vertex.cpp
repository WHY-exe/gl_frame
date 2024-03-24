#include "vertex.h"

#include "GL/glew.h"
#include "error.h"

namespace gl {
namespace vertex {

Buffer::Buffer(uint32_t count) noexcept : m_buffer_object(0) {
  // TODO: exception handling
  Init(count);
}

Buffer::~Buffer() noexcept {
  if (IsInit()) {
    glDeleteBuffers(1, &m_buffer_object);
  }
}

bool Buffer::Init(uint32_t count) noexcept {
  // generate one buffer object and return a uint
  // to represent buffer
  GLCall(glGenBuffers(count, &m_buffer_object));
  return IsInit();
}

void Buffer::Bind(const std::vector<float>& vertex_data) noexcept {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_buffer_object));
  // copy the vertices data to the buffer object
  // with the specific type that currently bound
  // to the pipline
  GLCall(glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float),
                      vertex_data.data(), GL_STATIC_DRAW));
}

bool Buffer::IsInit() const noexcept { return m_buffer_object != 0; }

Layout::Layout(uint32_t count) noexcept : m_vao(0) {
  // TODO: error handling
  Init(count);
}

Layout::~Layout() noexcept {
  if (IsInit()) {
    glDeleteVertexArrays(1, &m_vao);
  }
}

bool Layout::Init(uint32_t count) noexcept {
  GLCall(glGenVertexArrays(count, &m_vao));
  return IsInit();
}

bool Layout::IsInit() const noexcept { return m_vao != 0; }

void Layout::Bind() noexcept { GLCall(glBindVertexArray(m_vao)); }

void Layout::SetAttribute(const LayoutAttri& attri) noexcept {
  GLCall(glVertexAttribPointer(attri.index, attri.size, attri.type,
                               attri.normalize, attri.stride, attri.offset));
  GLCall(glEnableVertexAttribArray(attri.index));
}

}  // namespace vertex
}  // namespace gl
