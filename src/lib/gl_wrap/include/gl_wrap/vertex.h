#pragma once
#include <cstdint>
#include <vector>

#include "bindable.h"
namespace gl::vertex {
class Buffer : public Bindable {
private:
    std::vector<float> buffer_;

public:
    ~Buffer() noexcept;

    explicit Buffer(uint32_t count = 1) noexcept;
    bool Init(uint32_t count = 1) noexcept;

    void SetBuffer(std::vector<float> &&vertex_data) noexcept;
    bool Bind() noexcept final;
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
public:
    ~Layout() noexcept;

    explicit Layout(uint32_t count = 1) noexcept;
    bool Init(uint32_t count = 1) noexcept;

    bool SetAttribute(const LayoutAttri &attri) noexcept;
    bool Bind() noexcept final;
};

} // namespace gl::vertex
