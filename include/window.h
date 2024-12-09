#ifndef WINDOW_H
#define WINDOW_H

#include <optional>
#include <queue>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils.h"


class Window {
public:
    Window();
    ~Window();
    Window(Window const&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window const&) = delete;
    Window& operator=(Window&&) = delete;

    void update();
    std::optional<Event> get_event();
    bool should_close();

public:
    static std::queue<Event> event_queue;

private:
    void load_pressed_keys();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

private:
    GLFWwindow *window;
};

#endif // WINDOW_H

