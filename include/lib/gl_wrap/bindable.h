#pragma once
#include <cstdint>
namespace gl {
class Bindable {
 protected:
  uint32_t identifier_;

 protected:
  inline uint32_t GetID() noexcept { return identifier_; }

 public:
  Bindable() : identifier_(0){};
  virtual ~Bindable() = default;
  virtual bool Bind() noexcept = 0;
  inline virtual bool IsInit() const noexcept { return identifier_ != 0; };
};

}  // namespace gl
