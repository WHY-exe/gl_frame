#include "exception.h"
#include "util.h"
#include <cstdio>
namespace util {
Exception::Exception(uint32_t line, const char *func, const char *msg, const char *type) noexcept
    : what_buffer_() {
    snprintf(&(*what_buffer_.begin()), MAX_BUFFER_SIZE, "[%s][%s:%d][%s]", type, func, line, msg);
}

const char *Exception::what() const noexcept {
    return what_buffer_.data();
}

} // namespace util
