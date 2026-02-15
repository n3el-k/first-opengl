#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

enum MOVEMENT_DIRECTIONS {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
public:
    // Vector Construction
    Camera(glm::vec3 position, glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), float startingYaw = -90.f, float startingPitch = 0.f, 
        float speed = 17.f, float mouseSens = 0.08f);

    // Position construction
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float startingYaw = -90.f, 
        float startingPitch = 0.f, float speed = 17.f, float mouseSens=0.08f);

    glm::mat4 getViewMatrix();
    void processKeyboard(MOVEMENT_DIRECTIONS direction, float deltaTime);
    void processMouse(float xOff, float yOff, GLboolean isPitchConstrained = true);    


    // Getters
    glm::vec3 getPos();
    glm::vec3 getFront();
    glm::vec3 getUp();
    glm::vec3 getRight();
    float getYaw();
    float getPitch();
private:

    void update();

    glm::vec3 pos;
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 upUnitVector;
    float yaw;
    float pitch;
    float speed;
    float sens;

};