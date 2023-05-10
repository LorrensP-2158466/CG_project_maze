//
// Created by Lorrens Pantelis on 10/05/2023.
//

#ifndef OPENGL_PROJECT_OBSTACLE_H
#define OPENGL_PROJECT_OBSTACLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "Model.h"
#include "ShaderProgram.h"
#include <common.h>

struct Obstacle{

    Obstacle(glm::vec3 pos)
        : _obstacle((ROOT_DEF_ + "/assets/models/obstacle_model.obj"_s).c_str())
        , _pos(pos){}



    void draw(ShaderProgram &shader){
        if (_destroyed) return;
        shader.use();
        auto model = glm::translate(glm::mat4(1.f), _pos);
        model = glm::scale(model, {1, 2, 1});
        model = glm::scale(model, glm::vec3(0.5));
        shader.setMat4("model", model);
        _obstacle.Draw(shader);
    }

    void destroyed(){
        _destroyed  = true;
    }

    Model _obstacle;
    glm::vec3 _pos;
    bool _destroyed = false;
};

#endif //OPENGL_PROJECT_OBSTACLE_H
