//
// Created by Lorrens Pantelis on 26/04/2023.
//

#ifndef OPENGL_PROJECT_VERTEX_H
#define OPENGL_PROJECT_VERTEX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




struct vertex{
    // some defualt colours
    constexpr static glm::vec3 colour_green {0.f, 1.f, 0.f};
    constexpr static glm::vec3 colour_red   {1.f, 0.f, 0.f};
    constexpr static glm::vec3 colour_blue  {0.f, 0.f, 1.f};
    constexpr static glm::vec3 colour_white {1.f};
    constexpr static glm::vec3 colour_pink  {1.f, 0.f, 1.f};


    glm::vec3 pos; // position
    glm::vec3 colour; // color

    vertex(glm::vec3 p, glm::vec3 c){
        pos = p;
        colour = c;
    }
};


#endif //OPENGL_PROJECT_VERTEX_H
