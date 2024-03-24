#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
namespace gl {
namespace vertex {
class Buffer {
 private:
  uint32_t m_buffer_object;

 public:
  explicit Buffer(uint32_t count = 1) noexcept;
  ~Buffer() noexcept;

  bool Init(uint32_t count = 1) noexcept;
  void Bind(const std::vector<float>& vertex_data) noexcept;
  bool IsInit() const noexcept;
};

struct LayoutAttri {
  uint32_t index;  // index: where layout store in the gpu memory
  uint32_t size;   // size: the size of the element a vertex contain
  uint32_t type;   // type: the type of the element
  bool normalize;  // normalize: should data be normalized to [-1.0,1.0]
  size_t stride;   // stride: size of vertex in byte
  void* offset;    // offset: the offset where position data begin in buffer
};

class Layout {
 private:
  uint32_t m_vao;

 public:
  explicit Layout(uint32_t count = 1) noexcept;
  ~Layout() noexcept;

  bool Init(uint32_t count = 1) noexcept;
  void Bind() noexcept;
  void SetAttribute(const LayoutAttri& attri) noexcept;
  bool IsInit() const noexcept;
};

}  // namespace vertex
}  // namespace gl