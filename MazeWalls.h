//
// Created by Lorrens Pantelis on 26/04/2023.
//

#ifndef OPENGL_PROJECT_MAZEWALLS_H
#define OPENGL_PROJECT_MAZEWALLS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Vertex.h"
#include "ShaderProgram.h"
#include <iostream>
#include <array>



class MazeWalls {
public:

    ~MazeWalls(){
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ebo);
    }

    MazeWalls()
        : vertices {
            vertex({-1.f,  -1.f, 1.f}, vertex::colour_white),  // 0: bottom left front
            vertex({ 1.f,  1.f,  1.f}, vertex::colour_white),  // 1: top right front
            vertex({ 1.F, -1.F,  1.f}, vertex::colour_white),  // 2: top left front
            vertex({ -1.F, 1.F,  1.f}, vertex::colour_white),  // 3: bottom right front
            vertex({-1.f,  -1.f, -1.f}, vertex::colour_white), // 4: bottom left back
            vertex({ 1.f,  1.f,  -1.f}, vertex::colour_white), // 5: top right back
            vertex({ 1.F, -1.F,  -1.f}, vertex::colour_white), // 6: top left back
            vertex({ -1.F, 1.F,  -1.f}, vertex::colour_white), // 7: bottom right back
        },
        indices{
            3, 1, 0, 1, 0, 2, // front face
            6, 5, 7, 6, 4, 7, // back face
            0, 2, 6, 4, 6, 0, // left face
            1, 3, 5, 5, 7, 3, // right face
            2, 1, 5, 5, 6, 1, // top face
            0, 3, 7, 7, 4, 0// bottom face
        },
        wall_shader(ShaderProgram("../wall.vert", "../wall.frag"))
        {
        for (int i = 0; i < 100 /4 ; i += 2){
            _wall_offsets.emplace_back(-2, 0, i);
            _wall_offsets.emplace_back(2, 0, i);
        }
        glUseProgram(wall_shader.program_id());
        unsigned int uniformBlockIndex = glGetUniformBlockIndex(wall_shader.program_id(), "PV_mats");
        glUniformBlockBinding(wall_shader.program_id(), uniformBlockIndex, 0); // 0 is binding point to the PV_mats
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_instance_vbo);
        glGenBuffers(1, &_ebo);
        glUseProgram(0);
        init();
    }


    void init(){
        // VBO of instances, must be done outside of the vao
        glBindBuffer(GL_ARRAY_BUFFER, _instance_vbo);
        glBufferData(GL_ARRAY_BUFFER, _wall_offsets.size() * sizeof(glm::vec3) , _wall_offsets.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(_vao);
        // VBO of vertices binding
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
        // VBO of indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(),GL_STATIC_DRAW);

        // bind to VAO
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 *sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, _instance_vbo);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(2, 1);
        glBindVertexArray(0);
    }

    void draw() const {
        glBindVertexArray(_vao);
        glUseProgram(wall_shader.program_id());
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        wall_shader.setMat4("model", model);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr, _wall_offsets.size());
        glBindVertexArray(0);
        glUseProgram(0);
    }
    void init_instance_vbo(){
        // VBO of instances
        glBindVertexArray(_vao);

        glBindVertexArray(0);
    }

    void bind_instances_to_wallvao() const  {
        glBindVertexArray(_vao);

        glBindVertexArray(0);
    }

    ShaderProgram wall_shader;
    std::array<vertex, 8>  vertices;
    std::vector<glm::vec3> _wall_offsets {};
    std::array<GLuint, 36> indices;
    GLuint _vbo;
    GLuint _instance_vbo;
    GLuint _vao;
    GLuint _ebo;

};


#endif //OPENGL_PROJECT_MAZEWALLS_H
