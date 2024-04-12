#include "util.h"

#include <spdlog/spdlog.h>

#include <cmath>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "exception.h"
#include "posix_compat.h"
namespace util {
std::string exec_cmd::GetLine(const std::string &cmd) {
  std::unique_ptr<FILE, std::function<void(FILE *)>> pfileStream(
      popen(cmd.c_str(), "r"), [](FILE *pProc) -> void {
        if (pProc) {
          pclose(pProc);
          pProc = nullptr;
        }
      });

  char resultBuffer[MAX_BUFFER_SIZE] = {0};
  if (pfileStream == nullptr) {
    THROW_SYSTEM_ERROR;
  }
  while (fgets(resultBuffer, MAX_BUFFER_SIZE, pfileStream.get())) {
    if (strlen(resultBuffer)) {
      char lastChar = resultBuffer[strlen(resultBuffer) - 1];
      resultBuffer[strlen(resultBuffer) - 1] =
          lastChar == '\n' ? '\0' : lastChar;
      break;
    }
  }
  return resultBuffer;
}

std::string exec_cmd::GetAll(const std::string &cmd) {
  std::unique_ptr<FILE, std::function<void(FILE *)>> pfileStream(
      popen(cmd.c_str(), "r"), [](FILE *pProc) -> void {
        if (pProc) {
          pclose(pProc);
          pProc = nullptr;
        }
      });
  char resultBuffer[MAX_BUFFER_SIZE] = {0};
  std::string strResult;
  if (pfileStream == nullptr) {
    THROW_SYSTEM_ERROR;
  }
  while (fgets(resultBuffer, MAX_BUFFER_SIZE, pfileStream.get())) {
    if (strlen(resultBuffer)) {
      strResult += resultBuffer;
    }
  }
  return strResult;
}

std::vector<std::string> Split(const std::string &target,
                               const std::string &pattern) {
  size_t last_start_pos = 0;
  std::vector<std::string> vecResult;
  for (size_t cur_pos = target.find(pattern, last_start_pos);
       cur_pos != std::string::npos;
       cur_pos = target.find(pattern, last_start_pos)) {
    vecResult.emplace_back(
        target.substr(last_start_pos, cur_pos - last_start_pos));
    last_start_pos = cur_pos + pattern.length();
  }
  vecResult.emplace_back(
      target.substr(last_start_pos, target.length() - last_start_pos));
  return vecResult;
}

std::string ParsePath(const std::string &path) {
  std::vector<std::string> vec_dir = Split(path, "/");
  std::list<std::string> liResult;
  std::string strResult;
  for (auto &i : vec_dir) {
    if (i == ".." && !liResult.empty()) {
      liResult.pop_back();
    } else if (!i.empty() && i != ".") {
      liResult.push_back(std::move(i));
    }
  }
  for (const auto &i : liResult) {
    strResult += "/" + i;
  }
  return strResult;
}
std::string RemoveExtraSpaceInStr(const std::string &origin_str) noexcept {
  bool bAlert = false;
  std::string szResult = "";
  for (auto i : origin_str) {
    if (i == '\t') {
      i = ' ';
    }
    if (!bAlert && i == ' ') {
      szResult += i;
      bAlert = true;
    }
    if (bAlert && i != ' ') {
      bAlert = false;
    }
    if (bAlert) {
      continue;
    }
    szResult += i;
  }
  return szResult;
}
std::string RemoveExtraCharInStr(const std::string &origin_str,
                                 char c) noexcept {
  bool bAlert = false;
  std::string szResult = "";
  for (auto i : origin_str) {
    if (!bAlert && i == c) {
      szResult += i;
      bAlert = true;
    }
    if (bAlert && i != c) {
      bAlert = false;
    }
    if (bAlert) {
      continue;
    }
    szResult += i;
  }
  return szResult;
}

bool ContainAlphaInStr(const std::string &target) noexcept {
  bool bRes = false;
  for (auto i : target) {
    if ((bRes = isalpha(i)) != 0) {
      break;
    }
  }
  return bRes;
}

uint64_t HexToDec(const std::string &hex) {
  std::unordered_map<char, size_t> umapHex;
  char cHexInit = 'A';
  char cDecInit = '0';
  const size_t iHexInit = 10;
  for (size_t i = 0; i < iHexInit; i++) {
    umapHex.insert(std::make_pair(cDecInit++, i));
  }
  for (size_t i = iHexInit; i < 16; i++) {
    umapHex.insert(std::make_pair(cHexInit++, i));
  }
  uint64_t iRet = 0;
  size_t x = hex.length() - 1;
  for (auto i : hex) {
    iRet += umapHex[i] * (uint64_t)pow(16.0, (double)x);
    x--;
  }
  return iRet;
}

std::string DecToHex(int dec) {
  std::stringstream ss;
  ss << std::hex << dec;
  return ss.str();
}

std::string RemoveCharInStr(const std::string &target, char ch) noexcept {
  std::string strRet = "";
  for (auto i : target) {
    if (i == ch) {
      continue;
    }
    strRet += i;
  }
  return strRet;
}

std::string RemoveCharInStr(const std::string &target,
                            const std::string &chs) noexcept {
  std::string strRet = "";
  for (auto i : target) {
    bool bPass = false;
    for (auto c : chs) {
      if (bPass) {
        break;
      }
      bPass = bPass || (c == i);
    }
    if (bPass) {
      continue;
    }
    strRet += i;
  }
  return strRet;
}
} // namespace util