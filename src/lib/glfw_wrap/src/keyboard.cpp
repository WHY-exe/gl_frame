#include "keyboard.h"
#include "GLFW/glfw3.h"
#include <cstdint>
#include <queue>

namespace glfw {
bool Keyboard::KeyIsPressed(uint8_t key_code) const noexcept {
  return key_stat_[key_code];
}

Keyboard::Event Keyboard::ReadKey() noexcept {
  if (!key_events_.empty()) {
    const auto event = key_events_.front();
    key_events_.pop();
    return event;
  }
  return Keyboard::Event();
}

char Keyboard::ReadChar() noexcept {
  if (!key_events_.empty()) {
    const auto ch = char_events_.front();
    key_events_.pop();
    return ch;
  }
  return 0;
}

void Keyboard::OnKey(uint32_t key_code, int action) noexcept {
  key_stat_[key_code] = (action == GLFW_PRESS);
  key_events_.push(Event((Event::Stat)action, key_code));
  TrimBuffer(key_events_);
}

void Keyboard::OnChar(char ch) noexcept {
  char_events_.push(ch);
  TrimBuffer(char_events_);
}

template <typename Ty>
void Keyboard::TrimBuffer(std::queue<Ty> &buffer) noexcept {
  while (buffer.size() > MAX_BUFFER_SIZE) {
    buffer.pop();
  }
}

} // namespace glfw