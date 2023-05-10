//
// Created by Lorrens Pantelis on 26/04/2023.
//

#ifndef OPENGL_PROJECT_FLOOR_H
#define OPENGL_PROJECT_FLOOR_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Model.h"
#include <iostream>
#include "ShaderProgram.h"


class Floor {
public:
    Floor()
        : _shader("../assets/shader.vert", "../assets/shader.frag")
        , floor("../assets/models/floor_model.obj"){
        _shader.use();
        unsigned int uniformBlockIndex = glGetUniformBlockIndex(_shader.program_id(), "PV_mats");
        glUniformBlockBinding(_shader.program_id(), uniformBlockIndex, 0); // 0 is binding point to the PV_mats
        auto model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3 {10, -1, 10});
        //model = glm::rotate(model, 360.f, glm::vec3(0, 3, 0));
        _shader.setMat4("model", model);
    };

    void init_shader(){
            glUseProgram(_shader.program_id());
            _shader.setInt("material.diffuse", 0);
            _shader.setInt("material.specular", 1);

            /// todo: view pos in
            //_shader.setVec3("viewPos", _camera._pos);
            _shader.setFloat("material.shininess", 32.0f);
            // point light 1
            _shader.setVec3("pointLights[0].position", pointLightPositions[0]);
            _shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
            _shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
            _shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
            _shader.setFloat("pointLights[0].constant", 1.0f);
            _shader.setFloat("pointLights[0].linear", 0.09f);
            _shader.setFloat("pointLights[0].quadratic", 0.032f);
            // point light 2
            _shader.setVec3("pointLights[1].position", pointLightPositions[1]);
            _shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
            _shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
            _shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
            _shader.setFloat("pointLights[1].constant", 1.0f);
            _shader.setFloat("pointLights[1].linear", 0.09f);
            _shader.setFloat("pointLights[1].quadratic", 0.032f);
    };
        void draw(){
            _shader.use();
            floor.Draw(_shader);
        }
    ShaderProgram _shader;
    Model floor;
    glm::vec3 pointLightPositions[2] = {
            glm::vec3(8.0f,  3.0f,  20.0f),
            glm::vec3(15.0f, 3.0f, 10.0f)
    };
};


#endif //OPENGL_PROJECT_FLOOR_H