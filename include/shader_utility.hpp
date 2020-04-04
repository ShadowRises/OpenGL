#ifndef SHADER_UTILITY_H
#define SHADER_UTILITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

enum ShaderType {
        NONE = -1,
        VERTEX,
        FRAGMENT
};

struct ShadersSources {
        std::string VertexShader;
        std::string FragmentShader;
};


ShadersSources parseShader(const char* filepath);
unsigned int compileShader(unsigned int type, const std::string& source);
unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

#endif /* SHADER_UTILITY_H */
