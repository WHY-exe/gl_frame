#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

#include "bindable.h"
#include "error.hpp"
#include "common/macro.h"

namespace gl {
class IndexBuffer final : public Bindable {

public:
    std::vector<uint32_t> buffer_;
    int                   count_;

public:
    DEFAULT_MOVE_CTOR(IndexBuffer);
    ~IndexBuffer() noexcept final;

    static Result<IndexBuffer> New(int count = 1) noexcept;
    gl::Result<void> BindBuffer(std::vector<uint32_t> indicies);
    size_t           GetBufferSize() const noexcept;
    gl::Result<void> Bind() noexcept final;

private:
    IndexBuffer() noexcept = default;
    DEL_COPY_CTOR(IndexBuffer);
};

} // namespace gl