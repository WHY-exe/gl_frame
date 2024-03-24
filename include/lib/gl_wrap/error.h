#pragma once
#include <cassert>
namespace gl {
void ClearError() noexcept;
bool CheckError(const char* file, const char* function, int line) noexcept;
}  // namespace gl

#define GLCall(x)   \
  gl::ClearError(); \
  x;                \
  assert(gl::CheckError(__FILE__, #x, __LINE__))
