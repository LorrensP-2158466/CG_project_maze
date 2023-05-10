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
#include <common.h>
#include "ShaderProgram.h"


class Floor {
public:
    Floor()
        : floor("../assets/models/floor_model.obj"){
    };

        void draw(ShaderProgram &shader){
            shader.use();
            auto model = glm::translate(glm::mat4(1.F), glm::vec3 {10, -1, 10});
            //model = glm::rotate(model, 360.f, glm::vec3(0, 3, 0));
            shader.setMat4("model", model);
            floor.Draw(shader);
        }
    Model floor;
    glm::vec3 pointLightPositions[2] = {
            glm::vec3(8.0f,  3.0f,  20.0f),
            glm::vec3(15.0f, 3.0f, 10.0f)
    };
};


#endif //OPENGL_PROJECT_FLOOR_H