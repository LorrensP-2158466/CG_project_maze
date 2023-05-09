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


class Floor {
public:
    Floor()
        : floor_shader("../assets/shader.vert", "../assets/shader.frag")
        , floor("../assets/models/floor_model.obj"){
        floor_shader.use();
        unsigned int uniformBlockIndex = glGetUniformBlockIndex(floor_shader.program_id(), "PV_mats");
        glUniformBlockBinding(floor_shader.program_id(), uniformBlockIndex, 0); // 0 is binding point to the PV_mats
        auto model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3 {10, -1, 10});
        //model = glm::rotate(model, 360.f, glm::vec3(0, 3, 0));
        floor_shader.setMat4("model", model);
    };

        void draw(){
            floor_shader.use();
            floor.Draw(floor_shader);
        }
    ShaderProgram floor_shader;
    Model floor;
};


#endif //OPENGL_PROJECT_FLOOR_H