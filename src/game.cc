#include "game.h"

#include <cmath>
#include <filesystem>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "shader.h"
#include "utils.h"


Game::Game() : ourShader{std::filesystem::path("shaders/shader.vs").c_str(),
                     std::filesystem::path("shaders/shader.fs").c_str()} 
{
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    ourShader.setFloat("textureWeight", texture_weight);

    glEnable(GL_DEPTH_TEST);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          static_cast<void*>(0));
    glEnableVertexAttribArray(0);
    // color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
    //                      reinterpret_cast<void*>(3* sizeof(float)));
    //glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    load_texture(texture1, "assets/container.jpg");
    load_texture(texture2, "assets/awesomeface.png");
}

Game::~Game() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
}

void Game::load_texture(unsigned &texture, std::filesystem::path const& location) {
    unsigned char *data;
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //float borderColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // load and generate the texture
    int width, height, nrChannels;
    data = stbi_load(location.c_str(), &width, &height, &nrChannels, 0);
    GLenum src_channel_type = nrChannels == 4 ? GL_RGBA : GL_RGB;

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, src_channel_type,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Game::process_event(Event const& event, float deltaTime)
{
    const float cameraSpeed = 2.5f * deltaTime;
    if (KeyEvent const *ev = std::get_if<KeyEvent>(&event)) {
        switch (*ev) {
            case KeyEvent::UP:
                texture_weight += 0.01f;
                if (texture_weight > 1.0f) {
                    texture_weight = 1.0f;
                }
                ourShader.use();
                ourShader.setFloat("textureWeight", texture_weight);
                break;
            case KeyEvent::DOWN:
                texture_weight -= 0.01f;
                if (texture_weight < 0.0f) {
                    texture_weight = 0.0f;
                }
                ourShader.use();
                ourShader.setFloat("textureWeight", texture_weight);
                break;
            case KeyEvent::W:
                cameraPos += cameraSpeed * cameraFront;
                break;
            case KeyEvent::S:
                cameraPos -= cameraSpeed * cameraFront;
                break;
            case KeyEvent::A:
                cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                break;
            case KeyEvent::D:
                cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                break;
            default:
                std::cerr << "KeyEvent not handled" << std::endl;
                break;
        }
    }
    else if (ScrollEvent const *ev = std::get_if<ScrollEvent>(&event)) {
        fov -= static_cast<float>(ev->yoffset);
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 90.0f)
            fov = 90.0f; 
    }
    else if (MouseEvent const *ev = std::get_if<MouseEvent>(&event)) {
        if (firstMouse) // initially set to true
        {
            lastX = ev->xpos;
            lastY = ev->ypos;
            firstMouse = false;
        }

        float xoffset = ev->xpos - lastX;
        float yoffset = lastY - ev->ypos; // reversed since y-coordinates range from bottom to top
        lastX = ev->xpos;
        lastY = ev->ypos;

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
          pitch =  89.0f;
        if(pitch < -89.0f)
          pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }
}

void Game::render(float deltaTime) {
    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    ourShader.use();

    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    ourShader.setMatrix4fv("view", glm::value_ptr(view));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(fov), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
    ourShader.setMatrix4fv("projection", glm::value_ptr(projection));

    glBindVertexArray(VAO);
    for(unsigned int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i; 
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        ourShader.setMatrix4fv("model", glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

