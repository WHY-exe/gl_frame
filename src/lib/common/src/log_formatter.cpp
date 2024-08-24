#include "log_formatter.h"
#include "spdlog/pattern_formatter.h"
#include "util.h"

#include <filesystem>
#include <memory>
#ifdef WIN32
#    include <Windows.h>
#    include <array>
#endif
namespace util::log {
void ModuleFlagFormatter::format(const spdlog::details::log_msg &log_msg, const std::tm &,
                                 spdlog::memory_buf_t           &dest) {
    const std::string module_name = util::log::GetModuleName(log_msg.source.filename);
    dest.append(&*module_name.begin(), &*(module_name.end() - 1));
}

std::unique_ptr<spdlog::custom_flag_formatter> ModuleFlagFormatter::clone() const {
    return std::make_unique<ModuleFlagFormatter>();
}

std::string GetModuleName(std::string_view file_path) {
    std::filesystem::path path(file_path);
    return (--(--(--path.end())))->string();
}

std::string GetFileName(std::string_view file_path) {
    return std::filesystem::path(file_path).filename().string();
}

#ifdef WIN32
std::string FormatWin32Error(uint32_t error_num) noexcept {
    std::string err("");
    if (error_num == 0) {
        error_num = GetLastError();
    }
    std::array<TCHAR, MAX_BUFFER_SIZE> lpBuffer{};
    if (0
        == FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                             | FORMAT_MESSAGE_IGNORE_INSERTS,
                         nullptr, error_num, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                         lpBuffer.data(), 0, nullptr)) {
        std::array<char, MAX_BUFFER_SIZE> tmp{};
        sprintf_s(tmp.data(), tmp.size(), "undefine error description(%d)", error_num);
        err = tmp.data();
    } else {
        err = lpBuffer.data();
    }
    return err;
}
#endif // WIN32
} // namespace util::log
