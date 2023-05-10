//
// Created by Lorrens Pantelis on 26/04/2023.
//

#ifndef OPENGL_PROJECT_MAZEGAME_H
#define OPENGL_PROJECT_MAZEGAME_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/vec3.hpp"
#include <vector>
#include <iostream>
#include "Camera.h"
#include "Floor.h"
#include "Skybox.h"
#include "Model.h"
#include "MazeWall.h"
#include "Lamp.h"
#include "ShaderProgram.h"
#include "Cursor.h"
#include "common.h"



const auto projection = glm::perspective(glm::radians(45.f), 800.0f / 600.0f, 0.1f,1000.0f);

class MazeGame {
    enum Type : char{
        FILLED,
        EMPTY
    };
    using MazeObject = std::vector<std::vector<Type>>;
public:
    ~MazeGame(){
        glfwTerminate();
    }
    MazeGame()
        : default_program("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\" + "assets\\shader.vert"_s, "C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\" + "assets\\shader.frag"_s)
        , instanced_program("C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\" + "assets\\shaderInstanced.vert"_s, "C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\" + "assets\\shader.frag"_s)
        , light_objects{
                    PointLight::default_light_with_pos(glm::vec3(8.0f, 3.0f, 20.0f)),
                    PointLight::default_light_with_pos(glm::vec3(15.0f, 3.0f, 10.0f)),
                    PointLight::default_light_with_pos(glm::vec3(2.0f, 3.0f, 2.0f)),
                    PointLight::default_light_with_pos(glm::vec3(4.0f, 3.0f, 9.0f)),
                    PointLight::default_light_with_pos(glm::vec3(12.0f, 3.0f, 15.0f)),
            }
    {

        load_matrix();
        init_maze_wall();
        init_ubo_mats();
        init_shader_programs();
    }

    void init_maze_wall(){
        // x is still x but the z component is here z;
        int x = 0;
        int y = 0;
        std::vector<glm::vec3> poss;
        for (const auto &row: _maze_model){
            for (const auto &object: row){
                if (object == FILLED){
                    poss.emplace_back(y, 0, x);
                }
                x++;
            }
            y++;
            x = 0;
        }
        maze_walls.set_instances(poss);
    }
    void load_matrix(){
        std::ifstream maze_text {"C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\" + "maze.txt"_s};
        std::stringstream text;
        text << maze_text.rdbuf();
        auto str_repr = text.str();
        auto itr = str_repr.begin();
        char c;
        while ((str_repr.end() != (itr))){
            c = *(itr++);
            std::vector<Type> temp;
            do {
                if (c == '#') temp.emplace_back(FILLED);
                else temp.emplace_back(EMPTY);
            }while((c = *(itr++)) != '\n');
            _maze_model.push_back(temp);
        }
    };

    // Init Uniform Buffer Object and set projection
    void init_ubo_mats(){
        // init UBO
        glGenBuffers(1, &_ubo_mv_mats);
        glBindBuffer(GL_UNIFORM_BUFFER, _ubo_mv_mats);
        glBufferData(GL_UNIFORM_BUFFER, 2* sizeof(glm::mat4) + sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ubo_mv_mats, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3));
        // perspective never changes so we already insert it into the buffer
        glBindBuffer(GL_UNIFORM_BUFFER, _ubo_mv_mats);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &projection[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void init_default_program(){
        // default
        default_program.use();
        default_program.setInt("material.diffuse", 0);
        default_program.setInt("material.specular", 1);
        default_program.setFloat("material.shininess", 32.0f);
        unsigned int uniformBlockIndex = glGetUniformBlockIndex(default_program.program_id(), "PV_mats");
        glUniformBlockBinding(default_program.program_id(), uniformBlockIndex, 0); // 0 is binding point to the PV_mats
        int i = 0;
        for (const auto& light: light_objects){
            auto n = std::to_string(i);
            default_program.setVec3("pointLights["+ n +"].position", light.position);
            default_program.setVec3("pointLights["+ n +"].ambient", light.ambient);
            default_program.setVec3("pointLights["+ n +"].diffuse", light.diffuse);
            default_program.setVec3("pointLights["+n+"].specular", light.specular);
            default_program.setFloat("pointLights["+n+"].constant", light.constant);
            default_program.setFloat("pointLights["+n+"].linear", light.linear);
            default_program.setFloat("pointLights["+n+"].quadratic", light.quadratic);
            i++;
        }
    }

    void init_instanced_program(){
        instanced_program.use();
        instanced_program.setInt("material.diffuse", 0);
        instanced_program.setInt("material.specular", 1);
        instanced_program.setFloat("material.shininess", 32.0f);
        auto uniformBlockIndex = glGetUniformBlockIndex(instanced_program.program_id(), "PV_mats");
        glUniformBlockBinding(instanced_program.program_id(), uniformBlockIndex, 0); // 0 is binding point to the PV_mats
        int i = 0;
        for (const auto& light: light_objects){
            auto n = std::to_string(i);
            instanced_program.setVec3("pointLights["+n+"].position", light.position);
            instanced_program.setVec3("pointLights["+n+"].ambient", light.ambient);
            instanced_program.setVec3("pointLights["+n+"].diffuse", light.diffuse);
            instanced_program.setVec3("pointLights["+n+"].specular", light.specular);
            instanced_program.setFloat("pointLights["+n+"].constant", light.constant);
            instanced_program.setFloat("pointLights["+n+"].linear", light.linear);
            instanced_program.setFloat("pointLights["+n+"].quadratic", light.quadratic);
            i++;
        }
    }

    void init_shader_programs() {
        init_default_program();
        init_instanced_program();
    }

    void Draw() {
        lamp.Draw(light_objects);
        floor.draw(default_program);
        maze_walls.draw(instanced_program);
        _cursor.draw();
    }

    void drawSkybox() {
        auto view = glm::mat4(glm::mat3(_camera.GetViewMatrix()));
        glBindBuffer(GL_UNIFORM_BUFFER, _ubo_mv_mats);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        skybox.Draw();
    }

    void update(float delta_t) {
        // we update the view matrix to the UBO
        _camera.update(delta_t);
        auto view = _camera.GetViewMatrix();
        auto viewPos = _camera._pos;
        glBindBuffer(GL_UNIFORM_BUFFER, _ubo_mv_mats);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, 2  * sizeof(glm::mat4), sizeof(glm::vec3), &viewPos[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

    }

    void process_keyboard_input(Camera_Movement direction, float delta_t)
    {
        auto temp_camera = _camera;
        temp_camera.ProcessKeyboard(direction, delta_t);
        if (!collisions_with_camera_and_wall(delta_t, temp_camera)){
            // movement dit not cause colision
            _camera.ProcessKeyboard(direction, delta_t);
        }
    }

    bool collisions_with_camera_and_wall(float delta_t, const Camera& camera){
        auto c_pos = camera._pos;
        // bounding box for camera
        glm::vec3 minc = c_pos - glm::vec3(0.15f);
        glm::vec3 maxc = c_pos + glm::vec3(0.15f);
        for (const auto& wall_pos: maze_walls._positions) {
            // bounding box for wall object
            glm::vec3 minp = wall_pos - MazeWall::wall_size / 2.f;
            glm::vec3 maxp = wall_pos + MazeWall::wall_size / 2.F;
            // compare bounding boxes
            auto inside = minp.x <= maxc.x && minc.x <= maxp.x &&
                          minp.y <= maxc.y && minc.y <= maxp.y &&
                          minp.z <= maxc.z && minc.z <= maxp.z;
            if (inside) {
                return true; // yeah, one collision is good enough
            }
        }
        return false;
    }

    Camera _camera;
    Cursor _cursor;
private:
    ShaderProgram instanced_program;
    ShaderProgram default_program;
    GLuint _ubo_lights;
    GLuint _ubo_mv_mats;
    MazeObject _maze_model;
    MazeWall maze_walls;
    Floor floor;
    Skybox skybox;
    Lamp lamp;
    // positions of the point lights
    std::array<PointLight, 5> light_objects;
};


#endif //OPENGL_PROJECT_MAZEGAME_H
