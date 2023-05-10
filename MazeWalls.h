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

#include "stb_image.h"



class MazeWalls {
public:

    ~MazeWalls() {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_ebo);
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_texture);
        glDeleteBuffers(1, &_instance_vbo);
    }

    MazeWalls()
            : _vertices{
                vertex(glm::vec3{0.0, 0.0, 1.0,},  vertex::colour_blue), // 0: bottom left front
                vertex(glm::vec3{1.0, 0.0, 1.0,},  vertex::colour_blue),  // 1: top right front
                vertex(glm::vec3{1.0, 1.0, 1.0,},  vertex::colour_blue), // 2: top left front
                vertex(glm::vec3{0.0, 1.0, 1.0,},  vertex::colour_blue), // 3: bottom right front
                vertex(glm::vec3{0.0, 0.0, 0.0,},  vertex::colour_blue), // 4: bottom left back
                vertex(glm::vec3{1.0, 0.0, 0.0,},  vertex::colour_blue),  // 5: top right back
                vertex(glm::vec3{1.0, 1.0, 0.0,},  vertex::colour_blue),// 6: top left back
                vertex(glm::vec3{0.0, 1.0, 0.0},   vertex::colour_blue),// 7: bottom right back
            }
            ,indices{
                    // front
                    0, 1, 2,
                    2, 3, 0,

                    // right
                    1, 5, 6,
                    6, 2, 1,

                    // back
                    7, 6, 5,
                    5, 4, 7,

                    // left
                    4, 0, 3,
                    3, 7, 4,

                    // bottom
                    4, 5, 1,
                    1, 0, 4,

                    // top
                    3, 2, 6,
                    6, 7, 3
            }
            ,_tex_coords{
                glm::vec2{ 1.0, 1.0 },
                glm::vec2{ 0.0, 1.0, },
                glm::vec2{ 0.0, 0.0, },
                glm::vec2{ 1.0, 0.0, },
                glm::vec2{ 0.0, 1.0, },
                glm::vec2{ 0.0, 0.0 },

            }
            , wall_shader(ShaderProgram("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\wall.vert", "C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\wall.frag"))
        {
        for (int i = 0; i < 100 ; i++){
            _wall_offsets.emplace_back(-2,  0, i);
            _wall_offsets.emplace_back(2,  0, i);
        }
        glUseProgram(wall_shader.program_id());
        unsigned int uniformBlockIndex = glGetUniformBlockIndex(wall_shader.program_id(), "PV_mats");
        glUniformBlockBinding(wall_shader.program_id(), uniformBlockIndex, 0); // 0 is binding point to the PV_mats
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_instance_vbo);
        glGenBuffers(1, &_ebo);
        glGenBuffers(1, &_vbo);
        glGenTextures(1, &_texture);
        glUseProgram(0);
        init();
    }

    void init_instances(const std::vector<glm::vec3> &positions){
        _wall_offsets = positions;
        glBindBuffer(GL_ARRAY_BUFFER, _instance_vbo);
        glBufferData(GL_ARRAY_BUFFER, _wall_offsets.size() * sizeof(glm::vec3) , _wall_offsets.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void init(){
        glBindVertexArray(_vao);

        // VBO of instances, must be done outside of the vao


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(),GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


        // VBO of indices
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(vertex), _vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6  * sizeof(float), (void*)(3 *sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER,0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, _instance_vbo);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(2, 1);

        glBindBuffer(GL_ARRAY_BUFFER, _texture);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);


        // textures
        glBindTexture(GL_TEXTURE_2D, _texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char *data = stbi_load("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\Red_brick_wall_texture.JPG", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
        wall_shader.use();
        wall_shader.setInt("texture1", 0);
        glBindVertexArray(0);
    }

    void draw() const {
        glUseProgram(wall_shader.program_id());
        glBindVertexArray(_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _texture);

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, glm::vec3(0, -1, 0));
        model = glm::scale(model, glm::vec3(2));
        wall_shader.setMat4("model", model);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr, _wall_offsets.size());
        glBindVertexArray(0);
        glUseProgram(0);
    }


    ShaderProgram wall_shader;
    std::array<glm::vec2, 6>  _tex_coords;
    std::array<vertex, 8>     _vertices;
    std::vector<glm::vec3> _wall_offsets {};
    std::array<GLuint, 36> indices;
    GLuint _instance_vbo;
    GLuint _vao;
    GLuint _vbo;
    GLuint _ebo;
    GLuint _texture;

};


#endif //OPENGL_PROJECT_MAZEWALLS_H
