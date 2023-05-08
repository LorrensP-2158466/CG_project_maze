//
// Created by Lorrens Pantelis on 08/05/2023.
//

#ifndef OPENGL_PROJECT_COMMON_H
#define OPENGL_PROJECT_COMMON_H
#include <string>

std::string operator""_s(const char* str, size_t len){
    return {str};
}

#endif //OPENGL_PROJECT_COMMON_H
