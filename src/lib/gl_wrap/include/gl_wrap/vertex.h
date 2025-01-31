#pragma once
#include <cstdint>
#include <vector>

#include "bindable.h"

namespace gl::vertex {
class Buffer : public Bindable {
private:
    std::vector<float> buffer_;
    int count_;

public:
    DEFAULT_MOVE_CTOR(Buffer);

    ~Buffer() noexcept override;
    static Result<Buffer> New(int count = 1) noexcept;

    void SetBuffer(std::vector<float> &&vertex_data) noexcept;
    Result<void> Bind() noexcept final;

private:
    Buffer() noexcept = default;
    DEL_COPY_CTOR(Buffer);

};

struct LayoutAttri {
    uint32_t index;     // index: where layout store in the gpu memory
    int32_t  size;      // size: the size of the element a vertex contain
    uint32_t type;      // type: the type of the element
    bool     normalize; // normalize: should data be normalized to [-1.0,1.0]
    int32_t  stride;    // stride: size of vertex in byte
    void    *offset;    // offset: the offset where position data begin in buffer
};

class Layout : public Bindable {
private:
    int count_;

public:
    DEFAULT_MOVE_CTOR(Layout);
    ~Layout() noexcept override;

    static Result<Layout> New(int count = 1) noexcept;

    Result<void> SetAttribute(const LayoutAttri &attri) noexcept;
    Result<void> Bind() noexcept final;

private:
    Layout() noexcept = default;
    DEL_COPY_CTOR(Layout);
};

} // namespace gl::vertex
