#include "image_loader.h"
#include "stb/stb_image.h"

namespace util {
ImageLoader::ImageLoader(const std::string_view &file_path) noexcept {
    Init(file_path);
}

ImageLoader::~ImageLoader() noexcept {
    if (buffer_ != nullptr) {
        stbi_image_free(buffer_);
    }
}

void ImageLoader::Init(const std::string_view &file_path) noexcept {
    buffer_ = stbi_load(file_path.data(), &width_, &height_, &comp_, 0);
}

bool ImageLoader::IsInit() const noexcept {
    return buffer_ != nullptr;
}

int32_t ImageLoader::GetWidth() const noexcept {
    return width_;
}

int32_t ImageLoader::GetHeight() const noexcept {
    return height_;
}

const uint8_t *ImageLoader::GetBuffer() const noexcept {
    return buffer_;
}

uint8_t *ImageLoader::GetBuffer() noexcept {
    return buffer_;
}

} // namespace util