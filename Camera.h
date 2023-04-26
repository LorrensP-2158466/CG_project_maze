

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};



// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 _pos;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _world_up;
    // euler Angles
    float Yaw = -80.f;
    float Pitch = 0.0f;
    // camera options
    float MovementSpeed = 0.1;
    float _mouse_sens = 0.1f;
    // float MouseSensitivity;

    // constructor with vectors
    Camera() : _front(glm::vec3(0.0f, 0.0f, -1.0f))
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));
        _pos      = glm::vec3(0.0f, 0.0f, -3.f);
        _world_up = glm::vec3(0.0f, 1.0f, 0.0f);
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    [[nodiscard]] glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(_pos, _pos + _front, {0, 1.f, 0.f});
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction)
    {
        float velocity = MovementSpeed;
        if (direction == FORWARD)
            _pos += _front * velocity;
        if (direction == BACKWARD)
            _pos -= _front * velocity;
        if (direction == LEFT)
            _pos -= _right * velocity;
        if (direction == RIGHT)
            _pos += _right * velocity;
        _pos.y = 0.f; // user can not fly, so we hold the pos_y to 0
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= _mouse_sens;
        yoffset *= _mouse_sens;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }


private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        _front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        _right = glm::normalize(glm::cross(_front, _world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        _up    = glm::normalize(glm::cross(_right, _front));
        std::cout << _front.x << " " << _front.y << " " << _front.z << std::endl;
    }
};
#endif

