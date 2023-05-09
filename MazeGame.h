//
// Created by Lorrens Pantelis on 26/04/2023.
//

#ifndef OPENGL_PROJECT_MAZEGAME_H
#define OPENGL_PROJECT_MAZEGAME_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp"
#include <vector>
#include <iostream>
#include "MazeWalls.h"
#include "Camera.h"
#include "Floor.h"
#include "Skybox.h"
#include "Model.h"
#include "MazeWall.h"

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
    MazeGame(){
        load_matrix();
        init_maze_wall();
        init_ubo_mats();
        skybox.setCubemapTexture(skybox.loadCubemap());
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
        std::ifstream maze_text {"../maze.txt"};
        std::stringstream text;
        text << maze_text.rdbuf();
        auto str_repr = text.str();
        auto itr = str_repr.begin();
        char c;
        while ( (c = *(itr++)) != '\0'){
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
        glBufferData(GL_UNIFORM_BUFFER, 2* sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ubo_mv_mats, 0, 2 * sizeof(glm::mat4));
        // perspective never changes so we already insert it into the buffer
        glBindBuffer(GL_UNIFORM_BUFFER, _ubo_mv_mats);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }



    void Draw() {
        floor.Draw();
        maze_walls.draw();
    }

    void drawSkybox() {
        auto view = glm::mat4(glm::mat3(_camera.GetViewMatrix()));
        glBindBuffer(GL_UNIFORM_BUFFER, _ubo_mv_mats);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        //skybox.Draw();
    }

    void update(float delta_t) {
        // we update the view matrix to the UBO
        _camera.update(delta_t);
        auto view = _camera.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, _ubo_mv_mats);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    Camera _camera;
private:
    GLuint _ubo_mv_mats;
    MazeObject _maze_model;
    MazeWall maze_walls;
    Floor floor;
    Skybox skybox;
};


#endif //OPENGL_PROJECT_MAZEGAME_H
