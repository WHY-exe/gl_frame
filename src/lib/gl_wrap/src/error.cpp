#include "error.h"

#include <sstream>

#include "GL/glew.h"
#include "spdlog/spdlog.h"
namespace gl {
void ClearError() noexcept {
  while (glGetError() != GL_NO_ERROR)
    ;
}
bool CheckError(const char* file, const char* function, int line) noexcept {
	while (GLenum err = glGetError())
	{
		spdlog::error("[OpenGL Error]({:#X}){}\n{}:{}", err, function, file, line);
		return false;
	}
	return true;
}
}  // namespace gl