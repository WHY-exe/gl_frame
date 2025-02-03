#pragma once
#include "common/macro.h"

namespace glfw {
class Context {
public:
    Context(int version_major, int version_minor);
    ~Context() noexcept;

private:
    DEL_COPY_CTOR(Context);
    DEL_MOVE_CTOR(Context);
};
} // namespace glfw