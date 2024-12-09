#include "utils.h"
#include "window.h"
#include "game.h"

std::queue<Event> Window::event_queue {};

int main()
{
    Window window {};
    Game game {};

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    // render loop
    // -----------
    while (!window.should_close())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        // inputs
        while (std::optional<Event> const& event = window.get_event()) {
            game.process_event(*event, deltaTime);
        }

        game.render(deltaTime);

        window.update();
    }

    return 0;
}

