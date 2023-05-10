

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
    RIGHT,
    JUMP,
};

// Camera class from learnOpenGL book by Joey de vries, THANKS!
// But modified to fit our needs to also represent a person;
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
    float MovementSpeed = 4.F;
    float _mouse_sens = 0.1f;

    // player attributes;
    bool _is_jumping = false;
    bool _is_falling = false;
    const float GRAV = 5.f;
    const float max_jump_height = 1.f;
    float velocity_y = 0.f;
    // float MouseSensitivity;

    // constructor with vectors
    Camera() : _front(glm::vec3(0.0f, 0.0f, -1.0f))
    {
        _pos      = glm::vec3(3.0f, 0.0f, 4.f);
        _world_up = glm::vec3(0.0f, 1.0f, 0.0f);
        updateCameraVectors();
    }
    void update(float delta_t){
        // yeah this is broken as fuck
        auto cur_y = _pos.y;
        if (_is_jumping && _pos.y <= max_jump_height){
            std::cout << "in jump\n";
            velocity_y -= GRAV * delta_t;
            cur_y += velocity_y * delta_t;
            _pos.y = cur_y;
            if (_pos.y > max_jump_height) {
                _is_jumping = false;
                _is_falling = true;
            }
        }
        // falling
        if (_is_falling && _pos.y > 0.f){
            std::cout << "in fall\n";
            velocity_y -= GRAV * delta_t;
            cur_y -= velocity_y * delta_t;
            _pos.y = cur_y;
            if (cur_y < 0.f){
                _is_falling = false;
                _pos.y = 0;
                velocity_y = 0;
            }
        }

    };

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    [[nodiscard]] glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(_pos, _pos + _front, {0, 1.f, 0.f});
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float delta_t)
    {
        float velocity = MovementSpeed * delta_t;
        if (direction == FORWARD)
            _pos += _front * velocity;
        if (direction == BACKWARD)
            _pos -= _front * velocity;
        if (direction == LEFT)
            _pos -= _right * velocity;
        if (direction == RIGHT)
            _pos += _right * velocity;
        if (direction == JUMP && !_is_falling && !_is_jumping){
            _is_jumping = true;
            velocity_y = 8.f;
        }
         //if (!_is_jumping && !_is_falling)
           //  _pos.y = 0.f; // user can not fly, so we hold the pos_y to 0
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
    }
};
#endif

