#pragma once
#include "GLFW/glfw3.h"
#include <bitset>
#include <cstdint>
#include <queue>

struct GLFWwindow;
namespace glfw {
class Keyboard {
    friend void WndKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod);
    friend void WndCharCallback(GLFWwindow *window, unsigned int codepoint);

private:
    struct Event {
        enum class Stat { INVALID = -1, RELEASED, PRESSED, REPEAT };
        Stat     stat;
        uint32_t key_code;
        explicit Event(Stat stat = Stat::INVALID, uint32_t key_code = 0) noexcept
            : stat(stat), key_code(key_code){};
    };
    // glfw functional keys are greater than 256
    // let's ignore that first...
    static constexpr uint32_t KEY_NUM         = 256u;
    static constexpr uint32_t MAX_BUFFER_SIZE = 16u;

private:
    std::bitset<KEY_NUM> key_stat_;
    std::queue<Event>    key_events_;
    std::queue<char>     char_events_;

public:
    Keyboard() noexcept                   = default;
    ~Keyboard() noexcept                  = default;
    Keyboard(const Keyboard &)            = delete;
    Keyboard &operator=(const Keyboard &) = delete;

    inline void ClearState() noexcept {
        key_stat_.reset();
    };
    bool  KeyIsPressed(uint8_t key_code) const noexcept;
    Event ReadKey() noexcept;
    char  ReadChar() noexcept;

private:
    void OnKey(uint32_t key_code, int action) noexcept;
    void OnChar(char ch) noexcept;
    template <typename Ty>
    static void TrimBuffer(std::queue<Ty> &buffer) noexcept;
};

} // namespace glfw