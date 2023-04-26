//
// Created by Lorrens Pantelis on 26/04/2023.
//

#ifndef OPENGL_PROJECT_MAZEWALL_H
#define OPENGL_PROJECT_MAZEWALL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Vertex.h"

std::vector wall_vertices = {
        // front face
        vertex({-1.f,  1.f, 0}, vertex::colour_white),
        vertex({ 1.f,  1.f, 0}, vertex::colour_white),
        vertex({-1.F, -1.F, 0}, vertex::colour_white),
        vertex({ -1.F, -1.F, 0}, vertex::colour_white),
        vertex({ 1.f,  1.f,  0}, vertex::colour_white),
        vertex({ 1.f, -1.F,  0}, vertex::colour_white),
};

class MazeWall {
public:

};


#endif //OPENGL_PROJECT_MAZEWALL_H
