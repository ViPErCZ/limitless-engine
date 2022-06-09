#pragma once

#include <limitless/core/context.hpp>

namespace Limitless {
    enum class MouseButton : uint8_t {
        Left = GLFW_MOUSE_BUTTON_LEFT,
        Right = GLFW_MOUSE_BUTTON_RIGHT,
        Middle = GLFW_MOUSE_BUTTON_MIDDLE
    };

    enum class InputState : uint8_t {
        Released = GLFW_RELEASE,
        Pressed = GLFW_PRESS,
        Repeat = GLFW_REPEAT
    };
    enum class Modifier : uint8_t { Shift = 1, Control = 2, Alt = 4, Super = 8, CapsLock = 16, NumLock = 32 };

    class FramebufferObserver {
    public:
        virtual void onFramebufferChange(glm::uvec2 size) = 0;

        virtual ~FramebufferObserver() = default;
    };

    class MouseClickObserver {
    public:
        virtual void onMouseClick(glm::dvec2 pos, MouseButton button, InputState state, Modifier modifier) = 0;

        virtual ~MouseClickObserver() = default;
    };

    class MouseMoveObserver {
    public:
        virtual void onMouseMove(glm::dvec2 pos) = 0;

        virtual ~MouseMoveObserver() = default;
    };

    class KeyObserver {
    public:
        virtual void onKey(int key, int scancode, InputState state, Modifier modifier) = 0;

        virtual ~KeyObserver() = default;
    };

    class CharObserver {
    public:
        virtual void onChar(uint32_t utf8) = 0;

        virtual ~CharObserver() = default;
    };

    class ScrollObserver {
    public:
        virtual void onScroll(glm::dvec2 pos) = 0;

        virtual ~ScrollObserver() = default;
    };

    class ContextEventObserver : public Context {
    protected:
        static inline std::unordered_map<GLFWwindow*, ContextEventObserver*> callbacks;

        std::vector<FramebufferObserver*> framebuffer_observers;
        std::vector<MouseClickObserver*> mouseclick_observers;
        std::vector<MouseMoveObserver*> mousemove_observers;
        std::vector<KeyObserver*> key_observers;
        std::vector<ScrollObserver*> scroll_observers;
        std::vector<CharObserver*> char_observers;

        static void framebufferCallback(GLFWwindow* win, int w, int h);
        static void mouseclickCallback(GLFWwindow* win, int button, int action, int modifiers);
        static void mousemoveCallback(GLFWwindow* win, double x, double y);
        static void keyboardCallback(GLFWwindow* win, int key, int scancode, int action, int modifiers);
        static void scrollCallback(GLFWwindow* win, double x, double y);
        static void charCallback(GLFWwindow* win, uint32_t utf);

        void onFramebufferChange(glm::uvec2 size);
        void onMouseClick(glm::dvec2 pos, MouseButton button, InputState state, Modifier modifier) const;
        void onMouseMove(glm::dvec2 pos) const;
        void onKey(int key, int scancode, InputState state, Modifier modifier) const;
        void onScroll(glm::dvec2 pos) const;
        void onChar(uint32_t utf8) const;
    public:
        ContextEventObserver(std::string_view title, glm::uvec2 size, const WindowHints& hints = WindowHints{});
        ~ContextEventObserver() override;

        ContextEventObserver(const ContextEventObserver&) = delete;
        ContextEventObserver& operator=(const ContextEventObserver&) = delete;

        ContextEventObserver(ContextEventObserver&&) noexcept = default;
        ContextEventObserver& operator=(ContextEventObserver&&) noexcept = default;

        void setStickyMouseButtons(bool value) const noexcept;
        void setStickyKeys(bool value) const noexcept;

        InputState getKey(int key) const noexcept;
        bool isPressed(int key) const noexcept;

        InputState getMouseButton(MouseButton button);

        void registerObserver(FramebufferObserver* obs);
        void registerObserver(MouseClickObserver* obs);
        void registerObserver(MouseMoveObserver* obs);
        void registerObserver(KeyObserver* obs);
        void registerObserver(ScrollObserver* obs);
        void registerObserver(CharObserver* obs);

        void unregisterObserver(FramebufferObserver* obs);
        void unregisterObserver(MouseClickObserver* obs);
        void unregisterObserver(MouseMoveObserver* obs);
        void unregisterObserver(KeyObserver* obs);
        void unregisterObserver(ScrollObserver* obs);
        void unregisterObserver(CharObserver* obs);
    };
}
