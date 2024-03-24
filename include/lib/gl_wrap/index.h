#pragma once
#include <cstdint>
#include <vector>
namespace gl {
namespace index {
class Buffer {
 private:
  uint32_t m_ebo;

 public:
  explicit Buffer(uint32_t count = 1) noexcept;
  ~Buffer() noexcept;

  void Init(uint32_t count = 1);
  bool IsInit() const noexcept;

  void Bind(const std::vector<uint32_t>& indicies) noexcept;
};

}  // namespace index

}  // namespace gl