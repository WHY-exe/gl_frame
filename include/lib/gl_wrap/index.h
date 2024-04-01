#pragma once
#include <cstdint>
#include <vector>

#include "bindable.h"
namespace gl {
class IndexBuffer : public Bindable {
 public:
  std::vector<uint32_t> buffer_;

 public:
  ~IndexBuffer() noexcept;

  explicit IndexBuffer(uint32_t count = 1) noexcept;
  bool Init(uint32_t count = 1);

  void SetBuffer(std::vector<uint32_t>&& indicies);
  size_t GetBufferSize() const noexcept;
  bool Bind() noexcept override final;
};

}  // namespace gl