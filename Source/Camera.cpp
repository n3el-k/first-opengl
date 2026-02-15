#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 upVec, float startingYaw, float startingPitch, float moveSpeed, float mouseSens)
{
    pos = position;
    upUnitVector = upVec;
    yaw = startingYaw;
    pitch = startingPitch;
    speed = moveSpeed;
    sens = mouseSens;

    update();

    std::cout << "Yaw: " << yaw << " Pitch: " << pitch << std::endl;
    std::cout << "Front: (" << front.x << ", " << front.y << ", " << front.z << ")" << std::endl;
    std::cout << "X: " << pos.x << " Y: " << pos.y << "Z: " << pos.z << std::endl;
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float startingYaw, float startingPitch, float moveSpeed, float mouseSens)
{
    pos = glm::vec3(posX, posY, posZ);
    upUnitVector = glm::vec3(upX, upY, upZ);
    yaw = startingYaw;
    pitch = startingPitch;
    speed = moveSpeed;
    sens = mouseSens;
    update();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(pos, pos + front, up);
}

void Camera::processKeyboard(MOVEMENT_DIRECTIONS direction, float deltaTime)
{
    if (direction == FORWARD)
    {
        pos += front * speed * deltaTime; 
    }
    if (direction == BACKWARD)
    {
        pos -= front * speed * deltaTime; 
    }
    if (direction == LEFT)
    {
        pos -= speed * deltaTime * glm::normalize(right); 
    }
    if (direction == RIGHT)
    {
        pos += speed * deltaTime * glm::normalize(right); 
    }

    if (direction == UP)
    {
        pos += speed * deltaTime * up;
    }

    if (direction == DOWN)
    {
        pos -= speed * deltaTime * up;
    }
}

void Camera::processMouse(float xOff, float yOff, GLboolean isPitchConstrained)
{
    xOff *= sens;
    yOff *= sens;

    yaw += xOff;
    pitch += yOff;

    if (isPitchConstrained)
    {
        pitch = glm::clamp(pitch, -89.9f, 89.9f);
    }
    update();
}

void Camera::update()
{
    // recalculate front, right, up vectors
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, upUnitVector));
    up = glm::normalize(glm::cross(right, front));
}

glm::vec3 Camera::getPos()
{
    return pos;
}

glm::vec3 Camera::getFront()
{
    return front;
}

glm::vec3 Camera::getUp()
{
    return up;
}

glm::vec3 Camera::getRight()
{
    return right;
}

float Camera::getYaw()
{
    return yaw;
}

float Camera::getPitch()
{
    return pitch;
}