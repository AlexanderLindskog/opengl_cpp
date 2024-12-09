#ifndef GAME_H
#define GAME_H

#include <filesystem>

#include <glm/glm.hpp>

#include "shader.h"
#include "utils.h"


class Game {
public:
    Game();
    ~Game();
    Game(Game const&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game const&) = delete;
    Game& operator=(Game&&) = delete;

    void process_event(Event const&, float);
    void render(float);
private:
    void load_texture(unsigned&, std::filesystem::path const&);

private:
    bool firstMouse = true;
    float lastX = 400, lastY = 300;
    float sensitivity = 0.03;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 45.0f;
        
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); 

    unsigned int VBO, VAO/*, EBO*/;

    float texture_weight = 0.5f;
    unsigned texture1, texture2;
    Shader ourShader;
    
    glm::vec3 cubePositions[10] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
};

#endif // GAME_H

