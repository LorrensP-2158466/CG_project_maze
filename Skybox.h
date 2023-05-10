//
// Created by Hidde Brands on 01/05/2023
//

#ifndef OPENGL_PROJECT_SKYBOX_H
#define OPENGL_PROJECT_SKYBOX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Skybox {
public:

    ~Skybox() {
        glDeleteVertexArrays(1, &_sky_vao);
        glDeleteBuffers(1, &_sky_vbo);
    }

    Skybox()
        : skybox_shader("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\skybox.vert", "C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\skybox.frag")
    {
        sky_vertices = {
                // positions
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f
        };
        glUseProgram(skybox_shader.program_id());
        unsigned int uniformBlockIndex = glGetUniformBlockIndex(skybox_shader.program_id(), "PV_mats");
        glUniformBlockBinding(skybox_shader.program_id(), uniformBlockIndex, 0); // 0 is binding point to the PV_mats
        init_sky_buffers();
        loadCubemap();
    }

    void init_sky_buffers(){
        glGenVertexArrays(1, &_sky_vao);
        glGenBuffers(1, &_sky_vbo);
        glBindVertexArray(_sky_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _sky_vbo);
        glBufferData(GL_ARRAY_BUFFER, sky_vertices.size() * sizeof(float), sky_vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    void Draw() const {
        glDepthMask(GL_FALSE);
        glBindVertexArray(_sky_vao);
        glUseProgram(skybox_shader.program_id());
        glm::mat4 skyboxModel = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        skybox_shader.setMat4("model", skyboxModel);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
    }

    void loadCubemap()
    {
        glGenTextures(1, &cubemapTexture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            auto data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
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

        skybox_shader.use();
        skybox_shader.setInt("skybox", 0);

    }

    std::vector<std::string> faces =
    {
        std::string ("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\skybox\\right.jpg"),
        std::string ("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\skybox\\left.jpg"),
        std::string ("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\skybox\\top.jpg"),
        std::string ("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\skybox\\bottom.jpg"),
        std::string ("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\skybox\\front.jpg"),
        std::string ("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\skybox\\back.jpg")
    };

    unsigned int cubemapTexture;
    std::vector<float> sky_vertices;
    GLuint _sky_vao;
    GLuint _sky_vbo;
    ShaderProgram skybox_shader;
};


#endif //OPENGL_PROJECT_SKYBOX_H