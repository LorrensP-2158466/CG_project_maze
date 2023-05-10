cmake_minimum_required(VERSION 3.21)
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
find_package(assimp REQUIRED)
message(STATUS "Found Assimp in ${ASSIMP_LIBRARIES} ")
find_package(IrrKlang REQUIRED)
message(STATUS "Found Assimp in ${IRRKLANG_LIBRARIES} ")


if (MSVC)
    set(LIBS glfw3 assimp IrrKlang)
    add_compile_definitions(ROOT_DEF_="C:\\Users\\hidde\\OneDrive\\Documenten\\Hidde Uhasselt\\2e Bach\\Computer Graphics\\CG_project_maze\\")
    add_compile_definitions(PATH_SEP_='\')
    set(CMAKE_CXX_STANDARD 20)
endif(MSVC)
if (APPLE)
    add_compile_definitions(ROOT_DEF_="../")
    add_compile_definitions(PATH_SEP_='/')
    set(LIBS glfw3 assimp::assimp "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif(APPLE)
add_library(GLAD "glad.c" Model.h Mesh.h includes/common.h MazeWall.h Cursor.h)
set(ALL_LIBS ${LIBS} GLAD)

include_directories(${CMAKE_SOURCE_DIR}/includes)
add_executable(opengl_project main.cpp stb_image.h main.cpp)
target_link_libraries(opengl_project ${ALL_LIBS})