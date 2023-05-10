//
// Created by Lorrens Pantelis on 08/05/2023.
//

#ifndef OPENGL_PROJECT_MAZEWALL_H
#define OPENGL_PROJECT_MAZEWALL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "Model.h"
#include "ShaderProgram.h"

class MazeWall {
public:
    constexpr static glm::vec3 wall_size {1};
    constexpr static glm::vec3 dx {0, 0, 1};
    constexpr static glm::vec3 dy {0, 0, 1};
    constexpr static glm::vec3 dz {0, 0, 1};

    MazeWall()
        : _wall("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\assets\\models\\wall_model.obj")
        , _shader("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\assets\\shaderInstanced.vert", "C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\assets\\shader.frag")
    {
        glUseProgram(_shader.program_id());
        unsigned int uniformBlockIndex = glGetUniformBlockIndex(_shader.program_id(), "PV_mats");
        glUniformBlockBinding(_shader.program_id(), uniformBlockIndex, 0); // 0 is binding point to the PV_mats

        init_shader();
    }

    void update(glm::vec3 campos) {
        camera_pos = campos;

        glUseProgram(_shader.program_id());
        _shader.setVec3("viewPos", camera_pos);
    }

    void init_shader() {
        glUseProgram(_shader.program_id());
        _shader.setInt("material.diffuse", 0);
        _shader.setInt("material.specular", 1);

        _shader.setVec3("viewPos", camera_pos);
        _shader.setFloat("material.shininess", 32.0f);
        // point light 1
        _shader.setVec3("pointLights[0].position", pointLightPositions[0]);
        _shader.setVec3("pointLights[0].ambient", 0.30f, 0.30f, 0.30f);
        _shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        _shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        _shader.setFloat("pointLights[0].constant", 1.0f);
        _shader.setFloat("pointLights[0].linear", 0.09f);
        _shader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        _shader.setVec3("pointLights[1].position", pointLightPositions[1]);
        _shader.setVec3("pointLights[1].ambient", 0.30f, 0.30f, 0.30f);
        _shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        _shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        _shader.setFloat("pointLights[1].constant", 1.0f);
        _shader.setFloat("pointLights[1].linear", 0.09f);
        _shader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        _shader.setVec3("pointLights[2].position", pointLightPositions[2]);
        _shader.setVec3("pointLights[2].ambient", 0.30f, 0.30f, 0.30f);
        _shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        _shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        _shader.setFloat("pointLights[2].constant", 1.0f);
        _shader.setFloat("pointLights[2].linear", 0.09f);
        _shader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        _shader.setVec3("pointLights[3].position", pointLightPositions[3]);
        _shader.setVec3("pointLights[3].ambient", 0.30f, 0.30f, 0.30f);
        _shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        _shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        _shader.setFloat("pointLights[3].constant", 1.0f);
        _shader.setFloat("pointLights[3].linear", 0.09f);
        _shader.setFloat("pointLights[3].quadratic", 0.032f);
        // point light 5
        _shader.setVec3("pointLights[4].position", pointLightPositions[4]);
        _shader.setVec3("pointLights[4].ambient", 0.30f, 0.30f, 0.30f);
        _shader.setVec3("pointLights[4].diffuse", 0.8f, 0.8f, 0.8f);
        _shader.setVec3("pointLights[4].specular", 1.0f, 1.0f, 1.0f);
        _shader.setFloat("pointLights[4].constant", 1.0f);
        _shader.setFloat("pointLights[4].linear", 0.09f);
        _shader.setFloat("pointLights[4].quadratic", 0.032f);
    }

    void draw(){
        _shader.use();
        _wall.Draw(_shader);
    }

    void set_instances(const std::vector<glm::vec3> positions){
        _amount = positions.size();
        std::vector<glm::mat4> instances{};
        for (auto pos : positions){
            auto model = glm::mat4(1.f);
            model = glm::translate(model, pos);
            model = glm::scale(model, {1, 2, 1});
            model = glm::scale(model, glm::vec3(0.5));
            instances.push_back(model);
        }
        auto model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(1, 0, 3));
        //model = glm::scale(model, glm::vec3(0.5, 1, 0.5));
        instances.push_back(model);
        glGenBuffers(1, &_instance_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, _instance_buffer);
        glBufferData(GL_ARRAY_BUFFER, _amount * sizeof(glm::mat4), instances.data(), GL_STATIC_DRAW);
        _positions = positions;
        _wall.set_instances(instances);
    }
    std::vector<glm::vec3> _positions;
    Model         _wall;
    ShaderProgram _shader;
    GLuint _instance_buffer;
    size_t _amount;
    glm::vec3 camera_pos;

    // positions of the point lights
    glm::vec3 pointLightPositions[5] = {
        glm::vec3(8.0f,  3.0f,  20.0f),
        glm::vec3(15.0f, 3.0f, 10.0f),
        glm::vec3(2.0f, 3.0f, 2.0f),
        glm::vec3(4.0f, 3.0f, 9.0f),
        glm::vec3(12.0f, 3.0f, 15.0f)
    };
};


#endif //OPENGL_PROJECT_MAZEWALL_H
