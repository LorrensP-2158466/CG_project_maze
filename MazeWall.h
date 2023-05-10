//
// Created by Lorrens Pantelis on 08/05/2023.
//

#ifndef OPENGL_PROJECT_MAZEWALL_H
#define OPENGL_PROJECT_MAZEWALL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "Model.h"
#include "ShaderProgram.h"
#include <common.h>

class MazeWall {
public:
    constexpr static glm::vec3 wall_size {1};
    constexpr static glm::vec3 dx {0, 0, 1};
    constexpr static glm::vec3 dy {0, 0, 1};
    constexpr static glm::vec3 dz {0, 0, 1};

    MazeWall()
        : _wall("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\assets\\models\\wall_model.obj"){}

    void draw(ShaderProgram &shader){
        shader.use();
        shader.setMat4("model", model);
        _wall.Draw(shader);
    }

    void set_instances(const std::vector<glm::vec3>& positions){
        _amount = positions.size();
        std::vector<glm::mat4> instances{};
        for (auto pos : positions){
            auto model = glm::mat4(1.f);
            model = glm::translate(model, pos);
            model = glm::scale(model, {1, 2, 1});
            model = glm::scale(model, glm::vec3(0.5));
            instances.push_back(model);
        }
        auto model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(1, 0, 3));
        //model = glm::scale(model, glm::vec3(0.5, 1, 0.5));
        instances.push_back(model);
        glGenBuffers(1, &_instance_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, _instance_buffer);
        glBufferData(GL_ARRAY_BUFFER, _amount * sizeof(glm::mat4), instances.data(), GL_STATIC_DRAW);
        _positions = positions;
        _wall.set_instances(instances);
    }
    std::vector<glm::vec3> _positions;
    Model         _wall;
    glm::mat4 model;
    GLuint _instance_buffer;
    size_t _amount;
    glm::vec3 camera_pos;

};



#endif //OPENGL_PROJECT_MAZEWALL_H
