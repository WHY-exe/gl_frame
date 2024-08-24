#pragma once

namespace glfw {
class Context {
public:
    Context(int version_major, int version_minor);
    ~Context() noexcept;
};
} // namespace glfw