#pragma once
#include <functional>
#include <GL/glew.h>
#include <system_error>
#include <tl/expected.hpp>

namespace gl {

enum ERRORCODE {
    SHADER_FILE_NOT_FOUND = -3,
    SHADER_ERROR,
    PROGRAM_ERROR,
};

struct Error {
    std::error_code code;
    std::string     extra_info;
};

template <typename T>
using Result = tl::expected<T, Error>;

void ClearError() noexcept;

class ErrorCat : public std::error_category {
public:
    static const ErrorCat &Get();

    std::string message(int code) const final;
    const char *name() const noexcept final;
};

Error MakeError(int64_t code, std::string extra = "");

template <typename Func, typename... Args>
auto CheckError(Func &&GLFx, Args &&...FxArgs) -> Result<std::invoke_result_t<Func, Args...>>
    requires std::is_invocable_v<Func, Args...>
{
    using GLFxReturnType = std::invoke_result_t<Func, Args...>;
    auto fx              = std::bind_front(std::forward<Func>(GLFx), FxArgs...);
    ClearError();
    GLenum err = GL_NO_ERROR;
    if constexpr (std::is_void_v<GLFxReturnType>) {
        fx();
        while ((err = glGetError())) {
            return tl::unexpected(MakeError(err));
        }
        return {};
    } else {
        GLFxReturnType func_ret = fx();
        while ((err = glGetError())) {
            return tl::unexpected(MakeError(err));
        }
        return func_ret;
    }
}
} // namespace gl

#define RET_IF_ERROR(expected)                                                                     \
    if (!(expected)) {                                                                             \
        return tl::unexpected((expected).error());                                                 \
    }
