//
// Created by Lorrens Pantelis on 26/04/2023.
//

#ifndef OPENGL_PROJECT_FLOOR_H
#define OPENGL_PROJECT_FLOOR_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Vertex.h"
#include <iostream>





class Floor {
public:

    ~Floor() {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
    }

    Floor()
        : floor_shader("../floor.vert", "../floor.frag")
    {
        vertices = {
                vertex({-1.f,  -1.f, 1.f}, vertex::colour_green),
                vertex({ 1.f,  -1.f, 1.f}, vertex::colour_green),
                vertex({-1.F, -1.F, -1.f}, vertex::colour_green),
                vertex({ -1.F, -1.F, -1.f}, vertex::colour_green),
                vertex({ 1.f,  -1.f, 1.f}, vertex::colour_green),
                vertex({ 1.f, -1.F, -1.f}, vertex::colour_green),
        };
        glUseProgram(floor_shader.program_id());
        unsigned int uniformBlockIndex = glGetUniformBlockIndex(floor_shader.program_id(), "PV_mats");
        glUniformBlockBinding(floor_shader.program_id(), uniformBlockIndex, 0); // 0 is binding point to the PV_mats
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        init();
    }

    void init() {
        glBindVertexArray(_vao);
        // VBO of vertices binding
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glUseProgram(0);
    }

    void Draw() const {
        glBindVertexArray(_vao);
        glUseProgram(floor_shader.program_id());
        glm::mat4 floorModel = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        floorModel = glm::translate(floorModel, glm::vec3(0, 0, -10.F)); // move
        floorModel = glm::scale(floorModel, glm::vec3(10, 1, 10));
        floor_shader.setMat4("model", floorModel);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }


    std::vector<vertex> vertices;
    GLuint _vbo;
    GLuint _vao;
    ShaderProgram floor_shader;
};


#endif //OPENGL_PROJECT_FLOOR_H