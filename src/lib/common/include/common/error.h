#pragma once
#include <system_error>
#include <variant>
#include <spdlog/fmt/bundled/format.h>

namespace util {
enum class Error { SUCCESSED, NO_IMPLEMENTED, STD_EXCEPTION, UNKNOWN_EXCEPTION };

// Define a custom error code category derived from std::error_category
class ErrorCategory : public std::error_category {
public:
    ErrorCategory() noexcept = default;
    // Return a short descriptive name for the category
    const char *name() const noexcept final;
    // Return what each enum means in text
    std::string message(int c) const final;
    // OPTIONAL: Allow generic error conditions to be compared to me
    std::error_condition default_error_condition(int c) const noexcept final;

    static ErrorCategory &get();
};

std::error_code MakeErrorCode(Error e);

using ErrorType = std::variant<std::error_code, std::exception_ptr>;

} // namespace util

template <>
class fmt::formatter<util::Error> {
public:
    constexpr auto parse(auto &context) {
        auto       iter{context.begin()};
        const auto end{context.end()};
        if (iter != end && *iter != '}') {
            throw fmt::format_error{"Invalid gl::Error format specifier."};
        }
        return iter;
    };

    auto format(const util::ErrorType &error, auto &context) {
        std::string decode_msg = std::visit(
            [](auto &&arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::error_code>) {
                    return arg.message();
                } else if constexpr (std::is_same_v<T, std::exception_ptr>) {
                    try {
                        std::rethrow_exception(arg);
                    } catch (const std::exception &e) {
                        return e.what();
                    } catch (...) {
                        return "unknown exception";
                    }
                } else {
                    assert("unsupported type");
                }
            },
            error);
        return fmt::format_to(context.out(), "{}", std::move(decode_msg));
    }
};