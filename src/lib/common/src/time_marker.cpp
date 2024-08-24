#include "time_marker.h"
using namespace std::chrono;
namespace util {
TimeMarker::TimeMarker() noexcept : recorded_time_(steady_clock::now()) {}

float TimeMarker::Mark() noexcept {
    const auto last_recorded_time      = recorded_time_;
    recorded_time_                     = steady_clock::now();
    const duration<float> passing_time = recorded_time_ - last_recorded_time;
    return passing_time.count();
}

float TimeMarker::Peek() const {
    return duration<float>(steady_clock::now() - recorded_time_).count();
}

} // namespace util
