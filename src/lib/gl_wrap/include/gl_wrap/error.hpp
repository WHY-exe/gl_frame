#pragma once
#include <functional>
#include <GL/glew.h>
#include <system_error>
#include <tl/expected.hpp>

#include <spdlog/fmt/bundled/format.h>

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
        while ((err = glGetError()) != GL_NO_ERROR) {
            return tl::unexpected(MakeError(err));
        }
        return {};
    } else {
        GLFxReturnType func_ret = fx();
        while ((err = glGetError()) != GL_NO_ERROR) {
            return tl::unexpected(MakeError(err));
        }
        return func_ret;
    }
}
} // namespace gl

template <>
class fmt::formatter<gl::Error> {
public:
    constexpr auto parse(auto &context) {
        auto       iter{context.begin()};
        const auto end{context.end()};
        if (iter != end && *iter != '}') {
            throw fmt::format_error{"Invalid gl::Error format specifier."};
        }

        return iter;
    };

    auto format(const gl::Error &gl_error, auto &context) {
        if (gl_error.extra_info.empty()) {
            return fmt::format_to(context.out(), "[{}]", gl_error.code.message());
        }
        return fmt::format_to(context.out(), "[{}] {}",
                              gl_error.code.message(), gl_error.extra_info);
    }
};

#define RET_IF_ERROR(expected)                                                                     \
    if (!(expected)) {                                                                             \
        return tl::unexpected((expected).error());                                                 \
    }
