//
// Created by Lorrens Pantelis on 26/04/2023.
//

#ifndef OPENGL_PROJECT_MAZE_H
#define OPENGL_PROJECT_MAZE_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp"
#include <vector>
#include <iostream>
#include "MazeWall.h"

class Maze {
public:
    ~Maze(){
        glDeleteBuffers(1, &_instance_vbo);

    }
    Maze() : wall{MazeWall()}{
        for (int i = 0; i < 100 /4 ; i += 2){
            _wall_offsets.emplace_back(i, 0, -1);
            _wall_offsets.emplace_back(i, 0, 1);
        }

        init_instance_vbo();
        wall.init();
        bind_instances_to_wallvao();
    }

    void init_instance_vbo(){
        // VBO of instances
        glGenBuffers(1, &_instance_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _instance_vbo);
        glBufferData(GL_ARRAY_BUFFER, _wall_offsets.size() * sizeof(glm::vec3) , _wall_offsets.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void bind_instances_to_wallvao() const  {
        glBindBuffer(GL_ARRAY_BUFFER, _instance_vbo);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(2, 1);
        glEnableVertexAttribArray(2);
    }

    void Draw() const {
        glBindVertexArray(wall._vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, _wall_offsets.size());
        glBindVertexArray(0);
    }

private:
    std::vector<glm::vec3> _wall_offsets {};
    GLuint _instance_vbo;
    MazeWall wall;
};


#endif //OPENGL_PROJECT_MAZE_H
