#pragma once
#include "bindable.h"
#include "common/image_loader.h"
#include <cstdint>
#include <vector>
namespace gl {
class Texture2D : public Bindable {
public:
    struct Filter {
        int name;
        int type;
    };

private:
    int32_t num_ = 0;

public:
    explicit Texture2D(int num = 1) noexcept;
    ~Texture2D() noexcept;
    bool Bind() noexcept override;
    bool SetParam(const std::vector<Filter> &filters) noexcept;
    bool SetData(const util::ImageLoader &loader) noexcept;
};
} // namespace gl