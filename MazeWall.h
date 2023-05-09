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

class MazeWall {
public:
    MazeWall()
        : _wall("../assets/models/wall_model.obj")
        , _shader("../assets/shaderInstanced.vert", "../assets/shader.frag")
    {
        glUseProgram(_shader.program_id());
        unsigned int uniformBlockIndex = glGetUniformBlockIndex(_shader.program_id(), "PV_mats");
        glUniformBlockBinding(_shader.program_id(), uniformBlockIndex, 0); // 0 is binding point to the PV_mats
    }

    void draw(){
        _shader.use();
        _wall.Draw(_shader);
    }

    void set_instances(const std::vector<glm::vec3> positions){
        _amount = positions.size();
        std::vector<glm::mat4> instances{};
        for (auto i = 0; i < _amount; i++){
            auto model = glm::mat4(1.f);
            model = glm::translate(model, positions[i]);
            instances.push_back(model);
        }
        glGenBuffers(1, &_instance_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, _instance_buffer);
        glBufferData(GL_ARRAY_BUFFER, _amount * sizeof(glm::mat4), instances.data(), GL_STATIC_DRAW);
        _wall.set_instances(instances);
    }

    Model         _wall;
    ShaderProgram _shader;
    GLuint _instance_buffer;
    size_t _amount;
};


#endif //OPENGL_PROJECT_MAZEWALL_H