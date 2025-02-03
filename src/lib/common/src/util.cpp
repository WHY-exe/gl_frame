#include "util.h"
#include "spdlog/fmt/bundled/core.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef WIN32
#    include <Windows.h>
#    define popen _popen
#    define pclose _pclose
#endif

namespace util {
std::optional<std::string> exec_cmd::GetLine(const std::string &cmd) noexcept {
    std::unique_ptr<FILE, std::function<void(FILE *)>> file(popen(cmd.c_str(), "r"),
                                                            [](FILE *pProc) -> void {
                                                                if (pProc) {
                                                                    pclose(pProc);
                                                                    pProc = nullptr;
                                                                }
                                                            });

    std::array<char, MAX_BUFFER_SIZE> result_buf{};
    if (file == nullptr) {
        return std::nullopt;
    }
    while (fgets(result_buf.data(), MAX_BUFFER_SIZE, file.get())) {
        if (strlen(result_buf.data())) {
            const size_t last_char_index   = strlen(result_buf.data()) - 1;
            char         last_char         = result_buf.at(last_char_index);
            result_buf.at(last_char_index) = last_char == '\n' ? '\0' : last_char;
            break;
        }
    }
    return result_buf.data();
}

std::optional<std::string> exec_cmd::GetAll(const std::string &cmd) noexcept {
    std::unique_ptr<FILE, std::function<void(FILE *)>> file(popen(cmd.c_str(), "r"),
                                                            [](FILE *pProc) -> void {
                                                                if (pProc) {
                                                                    pclose(pProc);
                                                                    pProc = nullptr;
                                                                }
                                                            });
    std::array<char, MAX_BUFFER_SIZE>                  result_buf{};
    std::string                                        result;
    if (file == nullptr) {
        return std::nullopt;
    }
    while (fgets(result_buf.data(), MAX_BUFFER_SIZE, file.get())) {
        if (strlen(result_buf.data())) {
            result += result_buf.data();
        }
    }
    return result;
}

std::vector<std::string> Split(const std::string &target, const std::string &pattern) {
    size_t                   last_start_pos = 0;
    std::vector<std::string> result{};
    for (size_t cur_pos = target.find(pattern, last_start_pos); cur_pos != std::string::npos;
         cur_pos        = target.find(pattern, last_start_pos)) {
        result.emplace_back(target.substr(last_start_pos, cur_pos - last_start_pos));
        last_start_pos = cur_pos + pattern.length();
    }
    result.emplace_back(target.substr(last_start_pos, target.length() - last_start_pos));
    return result;
}

std::string RemoveExtraSpaceInStr(const std::string &origin_str) {
    bool        alert  = false;
    std::string result = "";
    for (auto i : origin_str) {
        if (i == '\t') {
            i = ' ';
        }
        if (!alert && i == ' ') {
            result += i;
            alert = true;
        }
        if (alert && i != ' ') {
            alert = false;
        }
        if (alert) {
            continue;
        }
        result += i;
    }
    return result;
}

std::string RemoveExtraCharInStr(const std::string &origin_str, char c) noexcept {
    bool        alert = false;
    std::string result{};
    std::copy_if(origin_str.begin(), origin_str.end(), result.begin(),
                 [c, &alert](char target_cur) -> bool {
                     if (!alert && target_cur == c) {
                         alert = true;
                     }
                     if (alert && target_cur != c) {
                         alert = false;
                     }
                     return !alert;
                 });
    return result;
}

bool ContainAlphaInStr(const std::string &target) noexcept {
    const auto itor = std::find_if(target.begin(), target.end(),
                                   [](char cur) -> bool { return isalpha(cur) != 0; });
    return itor != target.end();
}

uint64_t HexToDec(const std::string &hex) {
    static constexpr uint8_t         hex_top = 16;
    std::unordered_map<char, size_t> hex_map{};
    char                             cHexInit = 'A';
    char                             dec_int  = '0';
    const size_t                     hex_int  = 10;
    for (size_t i = 0; i < hex_int; i++) {
        hex_map.insert(std::make_pair(dec_int++, i));
    }
    for (size_t i = hex_int; i < hex_top; i++) {
        hex_map.insert(std::make_pair(cHexInit++, i));
    }
    uint64_t result = 0;
    size_t   x      = hex.length() - 1;
    for (auto i : hex) {
        result +=
            hex_map[i]
            * static_cast<uint64_t>(pow(static_cast<double>(hex_top), static_cast<double>(x)));
        x--;
    }
    return result;
}

std::string DecToHex(int dec) {
    return fmt::format("{:#X}", dec);
}

std::string RemoveCharInStr(const std::string &target, char ch) noexcept {
    std::string strRet{};
    std::copy_if(target.begin(), target.end(), strRet.begin(),
                 [ch](char target_cur) -> bool { return target_cur != ch; });
    return strRet;
}

std::string RemoveCharInStr(const std::string &target, const std::string &chs) noexcept {
    std::string strRet{};
    std::copy_if(target.begin(), target.end(), strRet.begin(), [&chs](char target_cur) -> bool {
        bool bPass = false;
        for (const auto i : chs) {
            if (bPass) {
                break;
            }
            bPass = bPass || (target_cur == i);
        }
        return !bPass;
    });
    return strRet;
}

#ifdef WIN32
std::string StrLastError(uint32_t error_num) noexcept {
    std::string err(MAX_BUFFER_SIZE, '\0');
    if (error_num == 0) {
        error_num = GetLastError();
    }
    if (0
        == FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                             | FORMAT_MESSAGE_IGNORE_INSERTS,
                         nullptr, error_num, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), &err[0], 0,
                         nullptr)) {
        sprintf_s(&err[0], err.size(), "undefine error description(%d)", error_num);
    }
    err.resize(err.find_first_of('\0'));
    return err;
}
#endif // WIN32
} // namespace util
