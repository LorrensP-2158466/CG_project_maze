//
// Created by Hidde Brands on 01/05/2023
//

#ifndef OPENGL_PROJECT_SKYBOX_H
#define OPENGL_PROJECT_SKYBOX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Vertex.h"
#include <iostream>
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"





class Skybox {
public:

    ~Skybox() {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
    }

    Skybox()
        : skybox_shader("../skybox.vert", "../skybox.frag")
    {
        vertices = {
            // positions          
            vertex({-1.0f,  1.0f, -1.0f}, vertex::colour_red),
            vertex({-1.0f, -1.0f, -1.0f}, vertex::colour_red),
            vertex({ 1.0f, -1.0f, -1.0f}, vertex::colour_red),
            vertex({ 1.0f, -1.0f, -1.0f}, vertex::colour_red),
            vertex({ 1.0f,  1.0f, -1.0f}, vertex::colour_red),
            vertex({-1.0f,  1.0f, -1.0f}, vertex::colour_red),

            vertex({-1.0f, -1.0f,  1.0f}, vertex::colour_red),
            vertex({-1.0f, -1.0f, -1.0f}, vertex::colour_red),
            vertex({-1.0f,  1.0f, -1.0f}, vertex::colour_red),
            vertex({-1.0f,  1.0f, -1.0f}, vertex::colour_red),
            vertex({-1.0f,  1.0f,  1.0f}, vertex::colour_red),
            vertex({-1.0f, -1.0f,  1.0f}, vertex::colour_red),

             vertex({1.0f, -1.0f, -1.0f}, vertex::colour_red),
             vertex({1.0f, -1.0f,  1.0f}, vertex::colour_red),
             vertex({1.0f,  1.0f,  1.0f}, vertex::colour_red),
             vertex({1.0f,  1.0f,  1.0f}, vertex::colour_red),
             vertex({1.0f,  1.0f, -1.0f}, vertex::colour_red),
             vertex({1.0f, -1.0f, -1.0f}, vertex::colour_red),

            vertex({-1.0f, -1.0f,  1.0f}, vertex::colour_red),
            vertex({-1.0f,  1.0f,  1.0f}, vertex::colour_red),
            vertex({ 1.0f,  1.0f,  1.0f}, vertex::colour_red),
            vertex({ 1.0f,  1.0f,  1.0f}, vertex::colour_red),
            vertex({ 1.0f, -1.0f,  1.0f}, vertex::colour_red),
            vertex({-1.0f, -1.0f,  1.0f}, vertex::colour_red),

            vertex({-1.0f,  1.0f, -1.0f}, vertex::colour_red),
            vertex({ 1.0f,  1.0f, -1.0f}, vertex::colour_red),
            vertex({ 1.0f,  1.0f,  1.0f}, vertex::colour_red),
            vertex({ 1.0f,  1.0f,  1.0f}, vertex::colour_red),
            vertex({-1.0f,  1.0f,  1.0f}, vertex::colour_red),
            vertex({-1.0f,  1.0f, -1.0f}, vertex::colour_red),

            vertex({-1.0f, -1.0f, -1.0f}, vertex::colour_red),
            vertex({-1.0f, -1.0f,  1.0f}, vertex::colour_red),
            vertex({ 1.0f, -1.0f, -1.0f}, vertex::colour_red),
            vertex({ 1.0f, -1.0f, -1.0f}, vertex::colour_red),
            vertex({-1.0f, -1.0f,  1.0f}, vertex::colour_red),
            vertex({ 1.0f, -1.0f,  1.0f}, vertex::colour_red)
        };
        glUseProgram(skybox_shader.program_id());
        unsigned int uniformBlockIndex = glGetUniformBlockIndex(skybox_shader.program_id(), "PV_mats");
        glUniformBlockBinding(skybox_shader.program_id(), uniformBlockIndex, 0); // 0 is binding point to the PV_mats
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
        glDepthMask(GL_FALSE);
        glBindVertexArray(_vao);
        glUseProgram(skybox_shader.program_id());
        glm::mat4 skyboxModel = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        skybox_shader.setMat4("model", skyboxModel);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
    }

    unsigned int loadCubemap()
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    void setCubemapTexture(unsigned int ct) {
        cubemapTexture = ct;
        skybox_shader.use();
        skybox_shader.setInt("skybox", ct); 
    }

    std::vector<std::string> faces =
    {
        std::string (ROOT_DEF_) + std::string ("skybox_ex/right.jpg"),
        std::string (ROOT_DEF_) + std::string ("skybox_ex/left.jpg"),
        std::string (ROOT_DEF_) + std::string ("skybox_ex/top.jpg"),
        std::string (ROOT_DEF_) + std::string ("skybox_ex/bottom.jpg"),
        std::string (ROOT_DEF_) + std::string ("skybox_ex/front.jpg"),
        std::string (ROOT_DEF_) + std::string ("skybox_ex/back.jpg")
    };

    unsigned int cubemapTexture;
    std::vector<vertex> vertices;
    GLuint _vbo;
    GLuint _vao;
    ShaderProgram skybox_shader;
};


#endif //OPENGL_PROJECT_SKYBOX_H