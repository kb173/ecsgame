//
// Created by karl on 03.01.20.
//

#ifndef ECSGAME_SHADER_H
#define ECSGAME_SHADER_H

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    /// Program ID
    unsigned int ID;

    /// Read and build the shader from the files at vertexPath and fragmentPath.
    /// Note that an OpenGL context has to be initialized before calling this! Otherwise a SIGSEGV will be thrown.
    Shader(const char *vertexPath, const char *fragmentPath);

    /// Activate the shader - usually called before rendering.
    void use();

    /// Set a uniform boolean in the shader
    void setBool(const std::string &name, bool value) const;

    /// Set a uniform int in the shader
    void setInt(const std::string &name, int value) const;

    /// Set a uniform float in the shader
    void setFloat(const std::string &name, float value) const;

    /// Set a uniform mat4 in the shader
    void setMat4(const std::string &name, glm::mat4 mat) const;

private:
    static void checkCompileErrors(unsigned int shader, const std::string &type);
};


#endif //ECSGAME_SHADER_H
