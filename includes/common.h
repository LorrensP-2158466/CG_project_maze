//
// Created by Lorrens Pantelis on 08/05/2023.
//

#ifndef OPENGL_PROJECT_COMMON_H
#define OPENGL_PROJECT_COMMON_H
#include <string>
#include <glm/glm.hpp>

std::string operator""_s(const char* str, size_t len){
    return {str};
}

[[maybe_unused]] void debug_print_vec3(glm::vec3 vec, const char* name){
    std::cout << name << ": x: " << vec.x
              << " y: " << vec.y
              << " z: " << vec.z << std::endl;
}

[[maybe_unused]] void debug_print_vec4(glm::vec4 vec, const char* name){
    std::cout << name << ": x: " << vec.x
              << " y: " << vec.y
              << " z: " << vec.z
              << " w: " << vec.w;
}

#endif //OPENGL_PROJECT_COMMON_H
