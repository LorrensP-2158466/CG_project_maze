//
// Created by Lorrens Pantelis on 26/04/2023.
//

#ifndef OPENGL_PROJECT_MAZEGAME_H
#define OPENGL_PROJECT_MAZEGAME_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp"
#include <vector>
#include <iostream>
#include "MazeWall.h"
#include "Camera.h"

const auto projection = glm::perspective(glm::radians(45.f), 800.0f / 600.0f, 0.1f,100.0f);

class MazeGame {
public:
    ~MazeGame(){
        glDeleteBuffers(1, &_instance_vbo);
        glfwTerminate();
    }
    MazeGame() : wall{MazeWall()}, _camera(){
        for (int i = 0; i < 100 /4 ; i += 2){
            _wall_offsets.emplace_back(-2, 0, i);
            _wall_offsets.emplace_back(2, 0, i);
        }
        wall.init();
        init_instance_vbo();
        bind_instances_to_wallvao();
        init_ubo_mats();

    }

    // Init Uniform Buffer Object and set projection
    void init_ubo_mats(){
        // init UBO
        glGenBuffers(1, &_ubo_mv_mats);
        glBindBuffer(GL_UNIFORM_BUFFER, _ubo_mv_mats);
        glBufferData(GL_UNIFORM_BUFFER, 2* sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ubo_mv_mats, 0, 2 * sizeof(glm::mat4));
        // perspective never changes so we already insert it into the buffer
        glBindBuffer(GL_UNIFORM_BUFFER, _ubo_mv_mats);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void init_instance_vbo(){
        // VBO of instances
        glBindVertexArray(wall._vao);
        glGenBuffers(1, &_instance_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _instance_vbo);
        glBufferData(GL_ARRAY_BUFFER, _wall_offsets.size() * sizeof(glm::vec3) , _wall_offsets.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    void bind_instances_to_wallvao() const  {
        glBindVertexArray(wall._vao);
        glBindBuffer(GL_ARRAY_BUFFER, _instance_vbo);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(2, 1);
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    void Draw() {
        update();
        wall.draw_many(_wall_offsets.size());
    }

    void update() {
        // we update the view matrix to the UBO
        auto view = _camera.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, _ubo_mv_mats);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    Camera _camera;
private:

    std::vector<glm::vec3> _wall_offsets {};
    GLuint _instance_vbo;
    GLuint _ubo_mv_mats;
    MazeWall wall;
};


#endif //OPENGL_PROJECT_MAZEGAME_H
