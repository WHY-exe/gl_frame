#pragma once
#include <cassert>
namespace gl {
void ClearError() noexcept;
bool CheckError(const char* file, const char* function, int line) noexcept;
}  // namespace gl

#define GLErrorInit bool gl_success = true

#define GLCall(x)   \
  gl::ClearError(); \
  x;                \
  gl_success = gl::CheckError(__FILE__, #x, __LINE__)

#define GLErrorResult gl_success
