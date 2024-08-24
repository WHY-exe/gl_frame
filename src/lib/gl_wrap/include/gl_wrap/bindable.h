#pragma once
#include <cstdint>
namespace gl {
class Bindable {
protected:
    uint32_t handle_;

protected:
    inline uint32_t GetID() noexcept {
        return handle_;
    }

public:
    Bindable() : handle_(0){};
    virtual ~Bindable()                 = default;
    virtual bool        Bind() noexcept = 0;
    inline virtual bool IsInit() const noexcept {
        return handle_ != 0;
    };
};

} // namespace gl
