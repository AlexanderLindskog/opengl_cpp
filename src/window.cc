#include "window.h"

#include <iostream>
#include <optional>
#include <queue>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils.h"


Window::Window() : window {}
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, 
                                          "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Window::framebuffer_size_callback);
    glfwSetCursorPosCallback(window, Window::mouse_callback);
    glfwSetScrollCallback(window, Window::scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

Window::~Window() {
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void Window::framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void Window::mouse_callback(GLFWwindow* /*window*/, double xpos, double ypos) {
    Window::event_queue.emplace(MouseEvent{xpos, ypos});
}

void Window::scroll_callback(GLFWwindow* /*window*/, double xoffset, double yoffset)
{
    Window::event_queue.emplace(ScrollEvent{xoffset, yoffset});
}

void Window::load_pressed_keys()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        event_queue.emplace(KeyEvent::UP);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        event_queue.emplace(KeyEvent::DOWN);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        event_queue.emplace(KeyEvent::W);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        event_queue.emplace(KeyEvent::S);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        event_queue.emplace(KeyEvent::A);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        event_queue.emplace(KeyEvent::D);
}

void Window::update() {
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
    glfwPollEvents();
    load_pressed_keys();
}

std::optional<Event> Window::get_event() {
    if (!event_queue.empty()) {
        Event ev = event_queue.front();
        event_queue.pop();
        return ev;
    }
    else {
        return {};
    }
}

bool Window::should_close() {
    return glfwWindowShouldClose(window);
}

