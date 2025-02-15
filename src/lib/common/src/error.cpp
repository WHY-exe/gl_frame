#include "error.h"
#include <cassert>
namespace util {

const char* ErrorCategory::name() const noexcept {
    return "UtilError";
}

std::string ErrorCategory::message(int c) const {
    switch (static_cast<Error>(c)) {
    case Error::SUCCESSED:
        return "no error";
    case Error::NO_IMPLEMENTED:
        return "no implemented";
    case Error::STD_EXCEPTION:
        return "std exception";
    case Error::UNKNOWN_EXCEPTION:
        return "catching unknown exception";
    default:
        return "unknown";
    }
}

std::error_condition ErrorCategory::default_error_condition(int c) const noexcept {
    switch (static_cast<Error>(c)) {
    case Error::NO_IMPLEMENTED:
        // return make_error_condition(std::errc::not_supported);
    default:
        return make_error_condition(std::errc::not_supported);
        break;
        // I have no mapping for this code
        // return std::error_condition(c, *this);
    }
}

ErrorCategory& ErrorCategory::get() {
    static ErrorCategory catagory;
    return catagory;
}

std::error_code MakeErrorCode(Error e) {
    return {static_cast<int>(e), ErrorCategory::get()};
}

}  // namespace util
