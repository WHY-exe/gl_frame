#pragma once
#include <cstdint>
#include <string_view>
namespace util {
class ImageLoader {
private:
    uint8_t *buffer_ = nullptr;
    int32_t  width_  = 0;
    int32_t  height_ = 0;
    int32_t  comp_   = 0;

public:
    ImageLoader() noexcept = default;
    ~ImageLoader() noexcept;
    explicit ImageLoader(const std::string_view &img_path) noexcept;
    void           Init(const std::string_view &file_path) noexcept;
    bool           IsInit() const noexcept;
    int32_t        GetWidth() const noexcept;
    int32_t        GetHeight() const noexcept;
    const uint8_t *GetBuffer() const noexcept;
    uint8_t       *GetBuffer() noexcept;
};
} // namespace util