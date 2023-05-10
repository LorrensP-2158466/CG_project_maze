//
// Created by Lorrens Pantelis on 09/05/2023.
//

#ifndef OPENGL_PROJECT_CURSOR_H
#define OPENGL_PROJECT_CURSOR_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include <array>


struct Cursor{
    constexpr static auto weight = glm::vec3(0.001, 0.1, 0);
    constexpr static auto weight_flipped = glm::vec3(0.1, 0.001, 0);
    ~Cursor(){
        glDeleteVertexArrays(1, &_vbo);
        glDeleteBuffers(1, &_vao);
    }

    Cursor()
        : shader("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\assets\\cursor.vert", "C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\assets\\cursor.frag")
        , vertices({
            // vertical
            glm::vec3(-1,  1, 0) * weight,
            glm::vec3(-1, -1, 0) * weight,
            glm::vec3( 1,  1, 0) * weight,
            glm::vec3( 1,  1, 0) * weight,
            glm::vec3( 1, -1, 0) * weight,
            glm::vec3(-1, -1, 0) * weight,

            // horizontal
            glm::vec3(-1,  1, 0) * weight_flipped,
            glm::vec3(-1, -1, 0) * weight_flipped,
            glm::vec3( 1,  1, 0) * weight_flipped,
            glm::vec3( 1,  1, 0) * weight_flipped,
            glm::vec3( 1, -1, 0) * weight_flipped,
            glm::vec3(-1, -1, 0) * weight_flipped,
        })
    {
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }


    void draw(){
        shader.use();
        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLES, 0, 12);
    }
    ShaderProgram shader;
    std::array<glm::vec3, 12> vertices;
    GLuint _vao, _vbo;

};

#endif //OPENGL_PROJECT_CURSOR_H
