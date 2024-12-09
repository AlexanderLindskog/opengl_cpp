#ifndef UTILS_H
#define UTILS_H

#include <variant>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct FrameBufferEvent {
    int width;
    int height;
};

struct MouseEvent {
    double xpos;
    double ypos;
};

struct ScrollEvent {
    double xoffset;
    double yoffset;
};

enum struct KeyEvent {
    W, A, S, D,
    UP, DOWN
};

using Event = std::variant<FrameBufferEvent,MouseEvent,ScrollEvent,KeyEvent>;


#endif // UTILS_H
