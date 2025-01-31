#pragma once

namespace glfw {
class Context {
public:
    Context(int version_major, int version_minor);
    ~Context() noexcept;

private:
    Context(const Context &)            = delete;
    Context &operator=(const Context &) = delete;
    Context(Context &&)                 = delete;
    Context &operator=(Context &&)      = delete;
};
} // namespace glfw