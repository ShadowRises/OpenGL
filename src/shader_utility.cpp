#include "shader_utility.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>

ShadersSources parseShader(const char* filepath) {
        FILE* shader = fopen(filepath, "r");
        if (!shader) {
                std::string message = "Can't open the file " + std::string(filepath);
                perror(message.c_str());
                exit(-1);
        }

        char line[256];
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;
        while(fgets(line, sizeof(line), shader)) {
                if (strstr(line, "#shader")) {
                        if (strstr(line, "vertex"))
                                type = ShaderType::VERTEX;
                        else if (strstr(line, "fragment"))
                                type = ShaderType::FRAGMENT;
                } else {
                        ss[type] << line << '\n';
                }
        }
        
        fclose(shader);
        return {ss[0].str(), ss[1].str()};
}

unsigned int compileShader(unsigned int type, const std::string& source) {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
                int length;
                glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
                char* message = (char*) alloca(length * sizeof(char));
                glGetShaderInfoLog(id, length, &length, message);
                std::cerr << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << message << std::endl;
                glDeleteShader(id);
                return 0;
        }
        
        return id;
}

unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
        unsigned int program = glCreateProgram();
        unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        // glDeleteShader(vs);
        // glDeleteShader(fs);
        glDetachShader(program, vs);
        glDetachShader(program, fs);

        return program;
}
