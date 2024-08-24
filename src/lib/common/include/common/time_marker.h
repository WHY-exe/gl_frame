#pragma once
#include <chrono>
namespace util {
class TimeMarker {
public:
    TimeMarker() noexcept;
    float Mark() noexcept;
    float Peek() const;

private:
    std::chrono::steady_clock::time_point recorded_time_;
};

} // namespace util
