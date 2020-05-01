#include "shader.hpp"

#include <glad/glad.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string &filepath)
    : vertex_shader(""), fragment_shader(""), is_bound(false)
{
        parse_shader(filepath.c_str());
        create_shaders();
}

Shader::Shader(const std::string &vertex,
               const std::string &fragment)
    : vertex_shader(vertex), fragment_shader(fragment), is_bound(false)
{
        create_shaders();
}

Shader::Shader(const std::string &str, Shader::Type type)
    : vertex_shader(""), fragment_shader(""), is_bound(false)
{
        switch (type)
        {
        case Shader::Type::VERTEX:
                vertex_shader = str;
                break;

        case Shader::Type::FRAGMENT:
                fragment_shader = str;
                break;

        default:
                break;
        }

        create_shaders();
}

void Shader::use()
{
        if (!is_bound)
        {
                glUseProgram(program_id);
                is_bound = true;
        }
}

unsigned int Shader::get_program_id() const
{
        return program_id;
}

void Shader::set_bool(const std::string &name, bool val) const
{
        glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)val);
}

void Shader::set_int(const std::string &name, int val) const
{
        glUniform1i(glGetUniformLocation(program_id, name.c_str()), val);
}

void Shader::set_float(const std::string &name, float val) const
{
        glUniform1f(glGetUniformLocation(program_id, name.c_str()), val);
}

void Shader::parse_shader(const char *filepath)
{
        FILE *shader = fopen(filepath, "r");
        if (!shader)
        {
                std::string message = "Can't open the file " + std::string(filepath);
                perror(message.c_str());
                exit(-1);
        }

        char line[256];
        std::stringstream ss[2];
        Shader::Type type = Shader::Type::NONE;
        while (fgets(line, sizeof(line), shader))
        {
                if (strstr(line, "#shader"))
                {
                        if (strstr(line, "vertex"))
                                type = Shader::Type::VERTEX;
                        else if (strstr(line, "fragment"))
                                type = Shader::Type::FRAGMENT;
                }
                else
                {
                        ss[type] << line << '\n';
                }
        }

        fclose(shader);

        vertex_shader = ss[Shader::Type::VERTEX].str();
        fragment_shader = ss[Shader::Type::FRAGMENT].str();
}

unsigned int Shader::compile_shader(unsigned int type,
                                    const std::string &source) const
{
        unsigned int id = glCreateShader(type);
        const char *src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
                int length;
                glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
                char *message = (char *)alloca(length * sizeof(char));
                glGetShaderInfoLog(id, length, &length, message);
                std::cerr << "ERROR::SHADER::"
                          << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
                          << "::COMPILATION_FAILED\n"
                          << message << std::endl;
                glDeleteShader(id);
                return 0;
        }

        return id;
}

void Shader::create_shaders()
{
        program_id = glCreateProgram();
        int vs = -1;
        int fs = -1;

        if (vertex_shader != "")
        {
                vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
                glAttachShader(program_id, vs);
        }

        if (fragment_shader != "")
        {
                fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
                glAttachShader(program_id, fs);
        }

        glLinkProgram(program_id);
        glValidateProgram(program_id);

        // glDeleteShader(vs);
        // glDeleteShader(fs);
        if (vs != -1)
                glDetachShader(program_id, vs);

        if (fs != -1)
                glDetachShader(program_id, fs);
}
