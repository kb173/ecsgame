//
// Created by karl on 03.01.20.
//

#ifndef ECSGAME_SHADER_H
#define ECSGAME_SHADER_H

#include <string>

class Shader {
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char *vertexPath, const char *fragmentPath);

    // use/activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

private:
    static void checkCompileErrors(unsigned int shader, const std::string &type);
};


#endif //ECSGAME_SHADER_H
