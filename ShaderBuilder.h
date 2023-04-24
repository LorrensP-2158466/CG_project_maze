//
// Created by Lorrens Pantelis on 24/04/2023.
//

#ifndef OPENGL_PROJECT_SHADERBUILDER_H
#define OPENGL_PROJECT_SHADERBUILDER_H

#include <glad/glad.h>
#include <string>


/**
 * Builds a shader
 */
class ShaderBuilder {

    ShaderBuilder(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
// utility uniform functions
    ShaderBuilder setBool(const std::string &name, bool value) const;
    ShaderBuilder setInt(const std::string &name, int value) const;
    ShaderBuilder setFloat(const std::string &name, float value) const;
};


#endif //OPENGL_PROJECT_SHADERBUILDER_H
