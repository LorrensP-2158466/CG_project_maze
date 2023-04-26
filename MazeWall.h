//
// Created by Lorrens Pantelis on 26/04/2023.
//

#ifndef OPENGL_PROJECT_MAZEWALL_H
#define OPENGL_PROJECT_MAZEWALL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Vertex.h"
#include <iostream>



class MazeWall {
public:

    ~MazeWall(){
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
    }

    MazeWall(){
        vertices = {
                vertex({-1.f,  1.f, 0}, vertex::colour_white),
                vertex({ 1.f,  1.f, 0}, vertex::colour_white),
                vertex({-1.F, -1.F, 0}, vertex::colour_white),
                vertex({ -1.F, -1.F, 0}, vertex::colour_white),
                vertex({ 1.f,  1.f,  0}, vertex::colour_white),
                vertex({ 1.f, -1.F,  0}, vertex::colour_white),
        };
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
    }

    void init(){
        glBindVertexArray(_vao);
        // VBO of vertices binding
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 *sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }


    std::vector<vertex> vertices;
    GLuint _vbo;
    GLuint _vao;
};


#endif //OPENGL_PROJECT_MAZEWALL_H
