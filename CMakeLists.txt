cmake_minimum_required(VERSION 3.2)
project(opengl_project)

set(CMAKE_CXX_STANDARD 17)

link_directories(${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(OpenGL REQUIRED)

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

set(LIBS glfw3 "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")

add_library(GLAD "glad.c" includes/KHR/khrplatform.h ShaderProgram.h MazeWall.cpp MazeWall.h Maze.cpp Maze.h Vertex.h)
set(LIBS ${LIBS} GLAD)

include_directories(${CMAKE_SOURCE_DIR}/includes)
add_executable(opengl_project main.cpp stb_image.h)
target_link_libraries(opengl_project ${LIBS})