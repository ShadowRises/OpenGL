#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <string>

class Shader
{
private:
        unsigned int program_id;
        std::string vertex_shader;
        std::string fragment_shader;
        bool is_bound;

public:
        enum Type
        {
                NONE = -1,
                VERTEX,
                FRAGMENT,
        };

        Shader() = delete;

        explicit Shader(const std::string &filepath);
        Shader(const std::string &vertex, const std::string &fragment);
        Shader(const std::string &str, Type);

        void use();

        unsigned int get_program_id() const;

        void set_bool(const std::string &name, bool val) const;
        void set_int(const std::string &name, int val) const;
        void set_float(const std::string &name, float val) const;
        void set_mat4(const std::string &name, const glm::mat4 &val) const;

private:
        void parse_shader(const char *filepath);

        unsigned int compile_shader(unsigned int type,
                                    const std::string &source) const;

        void create_shaders();
};

#endif /* SHADER_H */
