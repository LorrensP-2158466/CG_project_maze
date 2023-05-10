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
#include "ShaderProgram.h"





class Floor {
public:

    ~Floor() {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
    }

    Floor() : _shader("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\assets\\shader.vert", "C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\assets\\shader.frag") {}

    Floor(ShaderProgram &shader)
        : _shader(shader)
    {
        
        glUseProgram(_shader.program_id());
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        init();
    }

    void init() {
        glBindVertexArray(_vao);
        // VBO of vertices binding
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glUseProgram(0);
    }

    void Draw(ShaderProgram &shader) const {
        glBindVertexArray(_vao);
        glUseProgram(shader.program_id());
        glm::mat4 floorModel = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        floorModel = glm::translate(floorModel, glm::vec3(22, 0, 0)); // move
        floorModel = glm::scale(floorModel, glm::vec3(22, 1, 50));
        _shader.setMat4("model", floorModel);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }


    float vertices[32] = {
        0.5f,  0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // 1
        0.5f,  0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 2
        -0.5f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };
    GLuint _vbo;
    GLuint _vao;
    ShaderProgram _shader;
};


#endif //OPENGL_PROJECT_FLOOR_H